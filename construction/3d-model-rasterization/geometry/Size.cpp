#include "Size.h"
#include "string_format.h"

using namespace std;

string Size::toString() const
{
    return format("(width: %.3f, height: %.3f, depth: %.3f)", width, height, depth);
}

ostream &operator<<(ostream &out, const Size &s)
{
    out << s.toString();
    return out;
}
