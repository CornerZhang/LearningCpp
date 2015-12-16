//
//  MultiMethods.h
//
//  Created by huazhang on 12/14/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#ifndef MultiMethods_h
#define MultiMethods_h

// Copyright 2014 Jarod42 aka Joris Dauphin

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <array>

namespace inf {
    // C++11
    // std::index_sequence, std::make_index_sequence be needed, in <tuple>
    
    // Generic visit_compositor
    // Do: using MyIVisitor = visit_compositor<Child1, Child2, ...>
    template <typename ... Ts> class visit_compositor;
    
    template <typename T, typename ... Ts>
    class visit_compositor<T, Ts...> : public visit_compositor<Ts...>
    {
    public:
        using tuple_type = std::tuple<T, Ts...>;
        using visit_compositor<Ts...>::visit;
        
        virtual void visit(const T &t) = 0;
    };
    
    template <typename T> class visit_compositor<T>
    {
    public:
        using tuple_type = std::tuple<T>;
        
        virtual void visit(const T &t) = 0;
    };
    
    namespace detail
    {
        
        // retrieve the index of T in Ts...
        template <typename T, typename ... Ts> struct get_index;
        
        template <typename T, typename ... Ts>
        struct get_index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};
        
        template <typename T, typename Tail,  typename ... Ts>
        struct get_index<T, Tail, Ts...> :
        std::integral_constant < std::size_t, 1 + get_index<T, Ts...>::value > {};
        
        // retrieve the index of T in Tuple<Ts...>
        template <typename T, typename Tuple> struct get_index_in_tuple;
        
        template <typename T, template <typename...> class C, typename ... Ts>
        struct get_index_in_tuple<T, C<Ts...>> : get_index<T, Ts...> {};
        
        // tuple_contain
        
        template <typename T, typename Tuple> struct tuple_contain;
        
        template <typename T, typename ... Ts>
        struct tuple_contain<T, std::tuple<T, Ts...>> : std::true_type {};
        
        template <typename T>
        struct tuple_contain<T, std::tuple<>> : std::false_type {};
        
        template <typename T, typename T1, typename ... Ts>
        struct tuple_contain<T, std::tuple<T1, Ts...>> :
        tuple_contain<T, std::tuple<Ts...>> {};
        
        // get element of a multiarray
        template <std::size_t I>
        struct multi_array_getter {
            template <typename T, std::size_t N>
            static constexpr auto get(const T &a, const std::array<std::size_t, N> &indices)
            -> decltype(multi_array_getter<I - 1>::get(a[indices[N - I]], indices))
            {
                return multi_array_getter<I - 1>::get(a[indices[N - I]], indices);
            }
        };
        
        template <>
        struct multi_array_getter<0> {
            template <typename T, std::size_t N>
            static constexpr auto get(const T &a, const std::array<std::size_t, N> &)
            -> decltype(a)
            {
                return a;
            }
        };
        
        // Provide an implementation of visitor
        // by forwarding to C implementation (which may be non virtual)
        template <typename IVisitor, typename C, typename...Ts> struct visitor_compositor_impl;
        
        template <typename IVisitor, typename C, typename T, typename...Ts>
        struct visitor_compositor_impl<IVisitor, C, T, Ts...> : visitor_compositor_impl<IVisitor, C, Ts...> {
            virtual void visit(const T &t) override { C::visit(t); }
        };
        
        template <typename IVisitor, typename C, typename T>
        struct visitor_compositor_impl<IVisitor, C, T> : IVisitor, C {
            virtual void visit(const T &t) override { C::visit(t); }
        };
        
        // helper to expand child type to visitor_compositor_impl
        template <typename IVisitor, typename C>
        struct visitor_compositor_implType;
        
        template <typename ... Ts, typename C>
        struct visitor_compositor_implType<visit_compositor<Ts...>, C> {
            using type = visitor_compositor_impl<visit_compositor<Ts...>, C, Ts...>;
        };
        
        template <template <typename> class F, typename Tuple>
        struct to_index_sequence;
        
        template <template <typename> class F, typename... Ts>
        struct to_index_sequence<F, std::tuple<Ts...>>
        {
            using type = std::index_sequence<F<Ts>::value...>;
        };
        
        template <typename Tuple, typename SeqFilter, typename...Res>
        struct filter_tuple;
        
        template <typename...Res>
        struct filter_tuple<std::tuple<>, std::index_sequence<>, Res...>
        {
            using type = std::tuple<Res...>;
        };
        
        template <typename T, typename... Ts, std::size_t...Is, typename...Res>
        struct filter_tuple<std::tuple<T, Ts...>, std::index_sequence<0, Is...>, Res...>
        {
            using type = typename filter_tuple< std::tuple<Ts...>,
            std::index_sequence<Is...>,
                                                Res...>::type;
        };
        
        template <typename T, typename... Ts, std::size_t I,  std::size_t...Is, typename...Res>
        struct filter_tuple<std::tuple<T, Ts...>, std::index_sequence<I, Is...>, Res...>
        {
            using type = typename filter_tuple< std::tuple<Ts...>,
            std::index_sequence<Is...>,
                                                Res..., T>::type;
        };
        
        template <typename T> struct BUG; // TODO : REMOVE
        
        // Create an multi array of pointer of function
        // (with all combinaisons of overload).
        template <typename Ret, typename F, typename Arg>
        class get_all_overload
        {
        private:
            template<std::size_t N, typename Tuple>
            struct restricted_tuple
            {
            private:
                constexpr static std::size_t ArgSize = std::tuple_size<Arg>::value;
                using baseArg = typename std::decay<typename std::tuple_element<ArgSize - N, Arg>::type>::type;
                template <typename T>
                using FilterDerivedOf = std::is_base_of<baseArg, T>;
                using FilterIndex = typename to_index_sequence<FilterDerivedOf, Tuple>::type;
            public:
                using type = typename filter_tuple<Tuple, FilterIndex>::type;
            };
            
            template <typename...Ts>
            struct functor {
                // function which will be in array.
                static Ret call(F &f, const Arg &arg)
                {
                    return call_helper(f, arg, std::make_index_sequence<sizeof...(Ts)>());
                }
            private:
                // The final dispatched function
                template <std::size_t ... Is>
                static Ret call_helper(F &f, const Arg &arg, std::index_sequence<Is...>)
                {
                    using RetTuple = std::tuple<Ts &...>;
                    return f(static_cast<typename std::tuple_element<Is, RetTuple>::type>(std::get<Is>(arg))...);
                }
            };
            
            // helper class to create the multi array of function pointer
            template <std::size_t N, typename Tuple, typename TupleForArg, typename...Ts>
            struct builder;
            
            template <typename Tuple, typename...Ts, typename...Ts2>
            struct builder<1, Tuple, std::tuple<Ts...>, Ts2...> {
                using RetType = std::array<Ret(*)(F &, const Arg &), sizeof...(Ts)>;
                
                static constexpr RetType build()
                {
                    return RetType { &functor<Ts2..., Ts>::call... };
                }
            };
            
            template <std::size_t N, typename Tuple, typename ...Ts, typename...Ts2>
            struct builder<N, Tuple, std::tuple<Ts...>, Ts2...> {
                using RecTuple = typename restricted_tuple<N - 1, Tuple>::type;
                
                template <typename T>
                    using RecType = builder<N - 1, Tuple, RecTuple, Ts2..., T>;
                
                using T0 = typename std::tuple_element<0u, std::tuple<Ts...>>::type;
                using RetType = std::array<decltype(RecType<T0>::build()), sizeof...(Ts)>;
                
                static constexpr RetType build()
                {
                    return RetType { RecType<Ts>::build()... };
                }
            };
            
        public:
            template <std::size_t N, typename VisitorTuple>
            static constexpr auto get()
            -> decltype(builder<N, VisitorTuple, typename restricted_tuple<N, VisitorTuple>::type>::build())
            {
                return builder<N, VisitorTuple, typename restricted_tuple<N, VisitorTuple>::type>::build();
            }
        };
        
        
        template <typename Ret, typename IVisitor, typename F, std::size_t N>
        class dispatcher
        {
        private:
            std::array<std::size_t, N> index;
            
            template<typename Arg>
            struct visitor_call_impl {
                template <typename T>
                using FilterDerivedOf = std::is_base_of<Arg, T>;
                
                using Tuple = typename IVisitor::tuple_type;
                using FilterIndex = typename to_index_sequence<FilterDerivedOf, Tuple>::type;
                using FilteredTuple = typename filter_tuple<Tuple, FilterIndex>::type;
                
                template <typename T>
                typename std::enable_if<tuple_contain<T, FilteredTuple>::value>::type
                visit(const T &) const
                {
                    *index = get_index_in_tuple<T, FilteredTuple>::value;
                }
                
                template <typename T>
                typename std::enable_if<!tuple_contain<T, FilteredTuple>::value>::type
                visit(const T &) const
                {
                    // should not be called.
                }
                
                void setIndexPtr(std::size_t &index) { this->index = &index; }
            private:
                std::size_t *index = nullptr;
            };
            
            template <std::size_t I, typename Tuple>
            void set_index(const Tuple &t)
            {
                using ElementType = typename std::decay<typename std::tuple_element<I, Tuple>::type>::type;
                using VisitorType = typename visitor_compositor_implType<IVisitor, visitor_call_impl<ElementType>>::type;
                VisitorType visitor;
                visitor.setIndexPtr(index[I]);
                
                std::get<I>(t).accept(visitor);
            }
        public:
            template <typename Tuple, std::size_t ... Is>
            Ret operator()(F &&f, const Tuple &t, std::index_sequence<Is...>)
            {
                const int dummy[] = {(set_index<Is>(t), 0)...};
                static_cast<void>(dummy); // silent the warning unused varaible
                constexpr auto a = get_all_overload < Ret, F &&, Tuple >::
                template get<sizeof...(Is), typename IVisitor::tuple_type>();
                auto func = multi_array_getter<N>::get(a, index);
                return (*func)(f, t);
            }
        };
        
    } // namespace detail
    
    template <typename Ret, typename Visitor, typename F, typename ... Ts>
    Ret dispatch(F &&f, Ts &...args)
    {
        constexpr std::size_t size = sizeof...(Ts);
        detail::dispatcher<Ret, Visitor, F&&, size> d;
        return d(std::forward<F>(f), std::tie(args...), std::make_index_sequence<size>());
    }
    
}   // end of ::inf


#endif /* MultiMethods_h */
