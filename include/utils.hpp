#pragma once

#include <X11/Xlib.h>
#include <string>

namespace XTau{

    template<typename T>
    struct Size{
        T width, height;

        Size() = default;
        Size(T w, T h) : width(w), height(h) {}
        std::string ToString() const;
    };

    template<typename T>
    struct Position{
        T x, y;

        Position() = default;
        Position(T x, T y) : x(x), y(y) {}

        std::string ToString() const;
    };

    template<typename T>
    struct Vector2D{
        T x, y;

        Vector2D() = default;
        Vector2D(T x, T y) : x(x), y(y) {}

        std::string ToString() const;
    };

    /// @brief Joins a container of elements into a single string, with elements seperated by a delimiter.
    /// Any element can be used as long as an operator << on ostream is defined
    template<typename Container>
    std::string Join(const Container& container, const std::string& delimiter);

    /// @brief Joins a container of elements into a single string, with elements seperated by a delimiter.
    /// The elements are converted to string using a converter function
    template<typename Container, typename Converter>
    std::string Join(const Container& container, const std::string& delimiter, Converter converter);

    /// @brief Returns a string representation of a built in type that we already have ostream support for 
    template<typename T>
    std::string ToString(const T& a);


    std::string XRequestCodeToString(uint8_t request_code);
    std::string XConfigureWindowValueMaskToString(unsigned long value_mask);
    std::string ToString(const XEvent& e);

    //* IMPLEMENTATIONS

    template<typename T>
    std::ostream& operator << (std::ostream& out, const Size<T>& size){
        return out << size.ToString();
    }

    template<typename T>
    std::ostream& operator << (std::ostream& out, const Position<T>& size){
        return out << size.ToString();
    }

    template<typename T>
    std::ostream& operator << (std::ostream& out, const Vector2D<T>& size){
        return out << size.ToString();
    }

    template<typename T>
    Vector2D<T> operator - (const Position<T>& a, const Position<T>& b){
        return Vector2D<T>(a.x - b.x, a.y - b.y);
    }

    template<typename T>
    Position<T> operator + (const Position<T>& a, const Vector2D<T>& v){
        return Position<T>(a.x + v.x, a.y + v.y);
    }

    template<typename T>
    Position<T> operator + (const Vector2D<T>& v, const Position<T>& a){
        return Position<T>(a.x + v.x, a.y + v.y);
    }

    template<typename T>
    Position<T> operator - (const Position<T>& a, const Vector2D<T>& v){
        return Position<T>(a.x - v.x, a.y - v.y);
    }

    template<typename T>
    Vector2D<T> operator - (const Size<T>& a, const Size<T>& b){
        return Vector2D<T>(a.width - b.width, a.height - b.height);
    }

    template<typename T>
    Size<T> operator + (const Size<T>& a, const Vector2D<T>& v){
        return Size<T>(a.width + v.x, a.height + v.y);
    }

    template<typename T>
    Size<T> operator + (const Vector2D<T>& v, const Size<T>& a){
        return Size<T>(a.width + v.x, a.height + v.y);
    }

    template<typename T>
    Size<T> operator - (const Size<T>& a, const Vector2D<T>& v){
        return Size<T>(a.width - v.x, a.height - v.y);
    }
}