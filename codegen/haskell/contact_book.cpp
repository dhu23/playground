#include "contact_book.h"
Contact::Contact():
    data_(),
    type_(-1)
{
}
bool Contact::isstd::optional<ByPhone>() const
{
    return type_ == 0;
}
bool Contact::isstd::optional<ByMail>() const
{
    return type_ == 1;
}
bool Contact::isstd::optional<ByEmail>() const
{
    return type_ == 2;
}
std::optional<ByPhone>& Contact::makestd::optional<ByPhone>()
{
    new (&data_.byPhone_) std::optional<ByPhone>;
    type_ = 0;
    return data_.byPhone_;
}
std::optional<ByMail>& Contact::makestd::optional<ByMail>()
{
    new (&data_.byMail_) std::optional<ByMail>;
    type_ = 1;
    return data_.byMail_;
}
std::optional<ByEmail>& Contact::makestd::optional<ByEmail>()
{
    new (&data_.byEmail_) std::optional<ByEmail>;
    type_ = 2;
    return data_.byEmail_;
}