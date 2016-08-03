#ifndef __BTREE_H__
#define __BTREE_H__

/*
===============================================================================

Balanced Search Tree

===============================================================================
*/

#include "Heap.h"

//#define BTREE_CHECK

namespace inf {
    template< typename objType, typename keyType >
    class BTreeNode {
    public:
        keyType						key;			// key used for sorting
        objType *					object;			// if != NULL pointer to object stored in leaf node
        BTreeNode *					parent;			// parent node
        BTreeNode *					next;			// next sibling
        BTreeNode *					prev;			// prev sibling
        int							numChildren;	// number of children
        BTreeNode *					firstChild;		// first child
        BTreeNode *					lastChild;		// last child
    };


    template< typename objType, typename keyType, int maxChildrenPerNode >
    class BTree {
    public:
        typedef BTreeNode<objType, keyType>	node_type;

        BTree( void );
        ~BTree( void );

        void							init( void );
        void							shutdown( void );

        BTreeNode<objType,keyType> *	add( objType *object, keyType key );						// add an object to the tree
        void							remove( BTreeNode<objType,keyType> *node );				// remove an object node from the tree

        objType *						find( keyType key ) const;									// find an object using the given key
        objType *						find_smallest_larger_equal( keyType key ) const;				// find an object with the smallest key larger equal the given key

        BTreeNode<objType,keyType> *	get_root( void ) const;										// returns the root node of the tree
        int								get_node_count( void ) const;									// returns the total number of nodes in the tree
        BTreeNode<objType,keyType> *	get_next( BTreeNode<objType,keyType> *node ) const;		// goes through all nodes of the tree
        BTreeNode<objType,keyType> *	get_next_leaf( BTreeNode<objType,keyType> *node ) const;	// goes through all leaf nodes of the tree

    private:
        BTreeNode<objType,keyType> *	root;
        BlockAlloc<BTreeNode<objType,keyType>,128>	nodeAllocator;

        BTreeNode<objType,keyType> *	alloc_node( void );
        void							free_node( BTreeNode<objType,keyType> *node );
        void							split_node( BTreeNode<objType,keyType> *node );
        BTreeNode<objType,keyType> *	merge_nodes( BTreeNode<objType,keyType> *node1, BTreeNode<objType,keyType> *node2 );

        void							check_tree_r( BTreeNode<objType,keyType> *node, int &numNodes ) const;
        void							check_tree( void ) const;
    };


    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTree<objType,keyType,maxChildrenPerNode>::BTree( void ) {
        assert( maxChildrenPerNode >= 2 );
        root = NULL;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTree<objType,keyType,maxChildrenPerNode>::~BTree( void ) {
        shutdown();
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::init( void ) {
        root = alloc_node();
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::shutdown( void ) {
        nodeAllocator.shutdown();
        root = NULL;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTreeNode<objType,keyType> *BTree<objType,keyType,maxChildrenPerNode>::add( objType *object, keyType key ) {
        BTreeNode<objType,keyType> *node, *child, *newNode;

        if ( root->numChildren >= maxChildrenPerNode ) {
            newNode = alloc_node();
            newNode->key = root->key;
            newNode->firstChild = root;
            newNode->lastChild = root;
            newNode->numChildren = 1;
            root->parent = newNode;
            split_node( root );
            root = newNode;
        }

        newNode = alloc_node();
        newNode->key = key;
        newNode->object = object;

        for ( node = root; node->firstChild != NULL; node = child ) {

            if ( key > node->key ) {
                node->key = key;
            }

            // find the first child with a key larger equal to the key of the new node
            for( child = node->firstChild; child->next; child = child->next ) {
                if ( key <= child->key ) {
                    break;
                }
            }

            if ( child->object ) {

                if ( key <= child->key ) {
                    // insert new node before child
                    if ( child->prev ) {
                        child->prev->next = newNode;
                    } else {
                        node->firstChild = newNode;
                    }
                    newNode->prev = child->prev;
                    newNode->next = child;
                    child->prev = newNode;
                } else {
                    // insert new node after child
                    if ( child->next ) {
                        child->next->prev = newNode;
                    } else {
                        node->lastChild = newNode;
                    }
                    newNode->prev = child;
                    newNode->next = child->next;
                    child->next = newNode;
                }

                newNode->parent = node;
                node->numChildren++;

#ifdef BTREE_CHECK
                check_tree();
#endif

                return newNode;
            }

            // make sure the child has room to store another node
            if ( child->numChildren >= maxChildrenPerNode ) {
                split_node( child );
                if ( key <= child->prev->key ) {
                    child = child->prev;
                }
            }
        }

        // we only end up here if the root node is empty
        newNode->parent = root;
        root->key = key;
        root->firstChild = newNode;
        root->lastChild = newNode;
        root->numChildren++;

#ifdef BTREE_CHECK
        check_tree();
#endif

        return newNode;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::remove( BTreeNode<objType,keyType> *node ) {
        BTreeNode<objType,keyType> *parent;

        assert( node->object != NULL );

        // unlink the node from it's parent
        if ( node->prev ) {
            node->prev->next = node->next;
        } else {
            node->parent->firstChild = node->next;
        }
        if ( node->next ) {
            node->next->prev = node->prev;
        } else {
            node->parent->lastChild = node->prev;
        }
        node->parent->numChildren--;

        // make sure there are no parent nodes with a single child
        for ( parent = node->parent; parent != root && parent->numChildren <= 1; parent = parent->parent ) {

            if ( parent->next ) {
                parent = merge_nodes( parent, parent->next );
            } else if ( parent->prev ) {
                parent = merge_nodes( parent->prev, parent );
            }

            // a parent may not use a key higher than the key of it's last child
            if ( parent->key > parent->lastChild->key ) {
                parent->key = parent->lastChild->key;
            }

            if ( parent->numChildren > maxChildrenPerNode ) {
                split_node( parent );
                break;
            }
        }
        for ( ; parent != NULL && parent->lastChild != NULL; parent = parent->parent ) {
            // a parent may not use a key higher than the key of it's last child
            if ( parent->key > parent->lastChild->key ) {
                parent->key = parent->lastChild->key;
            }
        }

        // free the node
        free_node( node );

        // remove the root node if it has a single internal node as child
        if ( root->numChildren == 1 && root->firstChild->object == NULL ) {
            BTreeNode<objType,keyType> *oldRoot = root;
            root->firstChild->parent = NULL;
            root = root->firstChild;
            free_node( oldRoot );
        }

#ifdef BTREE_CHECK
        check_tree();
#endif
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    objType *BTree<objType,keyType,maxChildrenPerNode>::find( keyType key ) const {
        BTreeNode<objType,keyType> *node;

        for ( node = root->firstChild; node != NULL; node = node->firstChild ) {
            while( node->next ) {
                if ( node->key >= key ) {
                    break;
                }
                node = node->next;
            }
            if ( node->object ) {
                if ( node->key == key ) {
                    return node->object;
                } else {
                    return NULL;
                }
            }
        }
        return NULL;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    objType *BTree<objType,keyType,maxChildrenPerNode>::find_smallest_larger_equal( keyType key ) const {
        BTreeNode<objType,keyType> *node;

        for ( node = root->firstChild; node != NULL; node = node->firstChild ) {
            while( node->next ) {
                if ( node->key >= key ) {
                    break;
                }
                node = node->next;
            }
            if ( node->object ) {
                if ( node->key >= key ) {
                    return node->object;
                } else {
                    return NULL;
                }
            }
        }
        return NULL;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTreeNode<objType,keyType> *BTree<objType,keyType,maxChildrenPerNode>::get_root( void ) const {
        return root;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    int BTree<objType,keyType,maxChildrenPerNode>::get_node_count( void ) const {
        return nodeAllocator.get_alloc_count();
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTreeNode<objType,keyType> *BTree<objType,keyType,maxChildrenPerNode>::get_next( BTreeNode<objType,keyType> *node ) const {
        if ( node->firstChild ) {
            return node->firstChild;
        } else {
            while( node && node->next == NULL ) {
                node = node->parent;
            }
            return node;
        }
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTreeNode<objType,keyType> *BTree<objType,keyType,maxChildrenPerNode>::get_next_leaf( BTreeNode<objType,keyType> *node ) const {
        if ( node->firstChild ) {
            while ( node->firstChild ) {
                node = node->firstChild;
            }
            return node;
        } else {
            while( node && node->next == NULL ) {
                node = node->parent;
            }
            if ( node ) {
                node = node->next;
                while ( node->firstChild ) {
                    node = node->firstChild;
                }
                return node;
            } else {
                return NULL;
            }
        }
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTreeNode<objType,keyType> *BTree<objType,keyType,maxChildrenPerNode>::alloc_node( void ) {
        BTreeNode<objType,keyType> *node = nodeAllocator.alloc();
        node->key = 0;
        node->parent = NULL;
        node->next = NULL;
        node->prev = NULL;
        node->numChildren = 0;
        node->firstChild = NULL;
        node->lastChild = NULL;
        node->object = NULL;
        return node;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::free_node( BTreeNode<objType,keyType> *node ) {
        nodeAllocator.free( node );
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::split_node( BTreeNode<objType,keyType> *node ) {
        int i;
        BTreeNode<objType,keyType> *child, *newNode;

        // allocate a new node
        newNode = alloc_node();
        newNode->parent = node->parent;

        // divide the children over the two nodes
        child = node->firstChild;
        child->parent = newNode;
        for ( i = 3; i < node->numChildren; i += 2 ) {
            child = child->next;
            child->parent = newNode;
        }

        newNode->key = child->key;
        newNode->numChildren = node->numChildren / 2;
        newNode->firstChild = node->firstChild;
        newNode->lastChild = child;

        node->numChildren -= newNode->numChildren;
        node->firstChild = child->next;

        child->next->prev = NULL;
        child->next = NULL;

        // add the new child to the parent before the split node
        assert( node->parent->numChildren < maxChildrenPerNode );

        if ( node->prev ) {
            node->prev->next = newNode;
        } else {
            node->parent->firstChild = newNode;
        }
        newNode->prev = node->prev;
        newNode->next = node;
        node->prev = newNode;

        node->parent->numChildren++;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    BTreeNode<objType,keyType> *BTree<objType,keyType,maxChildrenPerNode>::merge_nodes( BTreeNode<objType,keyType> *node1, BTreeNode<objType,keyType> *node2 ) {
        BTreeNode<objType,keyType> *child;

        assert( node1->parent == node2->parent );
        assert( node1->next == node2 && node2->prev == node1 );
        assert( node1->object == NULL && node2->object == NULL );
        assert( node1->numChildren >= 1 && node2->numChildren >= 1 );

        for ( child = node1->firstChild; child->next; child = child->next ) {
            child->parent = node2;
        }
        child->parent = node2;
        child->next = node2->firstChild;
        node2->firstChild->prev = child;
        node2->firstChild = node1->firstChild;
        node2->numChildren += node1->numChildren;

        // unlink the first node from the parent
        if ( node1->prev ) {
            node1->prev->next = node2;
        } else {
            node1->parent->firstChild = node2;
        }
        node2->prev = node1->prev;
        node2->parent->numChildren--;

        free_node( node1 );

        return node2;
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::check_tree_r( BTreeNode<objType,keyType> *node, int &numNodes ) const {
        int numChildren;
        BTreeNode<objType,keyType> *child;

        numNodes++;

        // the root node may have zero children and leaf nodes always have zero children, all other nodes should have at least 2 and at most maxChildrenPerNode children
        assert( ( node == root ) || ( node->object != NULL && node->numChildren == 0 ) || ( node->numChildren >= 2 && node->numChildren <= maxChildrenPerNode ) );
        // the key of a node may never be larger than the key of it's last child
        assert( ( node->lastChild == NULL ) || ( node->key <= node->lastChild->key ) );

        numChildren = 0;
        for ( child = node->firstChild; child; child = child->next ) {
            numChildren++;
            // make sure the children are properly linked
            if ( child->prev == NULL ) {
                assert( node->firstChild == child );
            } else {
                assert( child->prev->next == child );
            }
            if ( child->next == NULL ) {
                assert( node->lastChild == child );
            } else {
                assert( child->next->prev == child );
            }
            // recurse down the tree
            check_tree_r( child, numNodes );
        }
        // the number of children should equal the number of linked children
        assert( numChildren == node->numChildren );
    }

    template< typename objType, typename keyType, int maxChildrenPerNode >
    void BTree<objType,keyType,maxChildrenPerNode>::check_tree( void ) const {
        int numNodes = 0;
        BTreeNode<objType,keyType> *node, *lastNode;

        check_tree_r( root, numNodes );

        // the number of nodes in the tree should equal the number of allocated nodes
        assert( numNodes == nodeAllocator.get_alloc_count() );

        // all the leaf nodes should be ordered
        lastNode = get_next_leaf( get_root() );
        if ( lastNode ) {
            for ( node = get_next_leaf( lastNode ); node; lastNode = node, node = get_next_leaf( node ) ) {
                assert( lastNode->key <= node->key );
            }
        }
    }

}   // ::inf

#endif /* !__BTREE_H__ */
