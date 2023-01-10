#include "../../include/Common/Coordinate.h"
#include <boost/unordered_map.hpp>

Coordinate::Coordinate():
    Row(-1), Column(-1){}

Coordinate::Coordinate(int const row, int const column):
    Row(row), Column(column){}

Coordinate::Coordinate(Coordinate const& other):
    Row(other.Row), Column(other.Column){}

Coordinate::Coordinate(Coordinate&& other) noexcept:
    Row(std::forward<int>(other.Row)), Column(std::forward<int>(other.Column)) {}

bool Coordinate::operator==(const Coordinate &other) const
{
    return Row == other.Row && Column == other.Column;
}

bool Coordinate::operator!=(const Coordinate &other) const
{
    return !(*this == other);
}

std::ostream& operator << (std::ostream& out,const Coordinate& coordinate)
{
    return out << '(' << coordinate.Row << ", " << coordinate.Column << ')';
}

Coordinate& Coordinate::operator=(Coordinate const& other)
{
    if(this != &other)
    {
        Row = other.Row;
        Column = other.Column;
    }
    return *this;
}

Coordinate& Coordinate::operator=(Coordinate&& other)
 noexcept {
    if(this != &other)
    {
        Row = std::forward<int>(other.Row);
        Column = std::forward<int>(other.Column);
    }
    return *this;
}

int Coordinate::GetRow(void) const {return Row;}
int Coordinate::GetColumn(void) const {return Column;}

std::size_t CoordinateHasher::operator()(const Coordinate& coordinate) const noexcept
{
    std::size_t seed = 0;
    boost::hash_combine(seed, coordinate.GetRow());
    boost::hash_combine(seed, coordinate.GetColumn());
    return seed;
}