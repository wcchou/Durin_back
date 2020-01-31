#ifndef DURIN_SCOPED_BASIC_SCOPED_HPP
#define DURIN_SCOPED_BASIC_SCOPED_HPP

#include <Durin/Config/Config.hpp>

#include <utility>

namespace Durin {

template <typename TypeTratis>
class BasicScopedObject {
public:
    using ValueType = typename TypeTratis::ValueType;

    explicit
    BasicScopedObject( const ValueType& value )
        : value_{ value }
    {
    }

    ~BasicScopedObject()
    {
        TypeTratis::close( value_ );
    }

    bool close()
    {
        return TypeTratis::close( value_ );
    }

    ValueType release()
    {
        ValueType result = value_;
        value_ = TypeTratis::InvalidValue;
        return result;
    }

    void reset( ValueType value )
    {
        close();
        value_ = value;
    }

    void swap( BasicScopedObject& rhs )
    {
        std::swap( value_, rhs.value_ );
    }

    bool isValid() const
    {
        return TypeTratis::isValid( value_ );
    }

    ValueType get()
    {
        return value_;
    }

private:
    ValueType  value_;
};

} // namespace Durin

#endif // End of include guard
