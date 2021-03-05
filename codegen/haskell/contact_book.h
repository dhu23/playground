#include <iostream>
#include <optional>
#include <vector>
class ByPhone
{
    std::string number_;
public:
    ByPhone() {}
    std::string& number() { return number_; }
    const std::string& number() const { return number_; }
    std::ostream& print(std::ostream& os) const;
};
class ByMail
{
    int streetNum_;
    std::string streetName_;
    std::string city_;
    std::string state_;
public:
    ByMail() {}
    int& streetNum() { return streetNum_; }
    const int& streetNum() const { return streetNum_; }
    std::string& streetName() { return streetName_; }
    const std::string& streetName() const { return streetName_; }
    std::string& city() { return city_; }
    const std::string& city() const { return city_; }
    std::string& state() { return state_; }
    const std::string& state() const { return state_; }
    std::ostream& print(std::ostream& os) const;
};
class ByEmail
{
    std::string useName_;
    std::string domain_;
public:
    ByEmail() {}
    std::string& useName() { return useName_; }
    const std::string& useName() const { return useName_; }
    std::string& domain() { return domain_; }
    const std::string& domain() const { return domain_; }
    std::ostream& print(std::ostream& os) const;
};
class Contact
{
    union U
    {
        std::optional<ByPhone> byPhone_;
        std::optional<ByMail> byMail_;
        std::optional<ByEmail> byEmail_;
        U() { new(&byPhone_) std::optional<ByPhone>(); }
    };
    U data_;
    int type_;
public:
    Contact();
    void reset() { type_ = -1; }
    bool isstd::optional<ByPhone>() const;
    bool isstd::optional<ByMail>() const;
    bool isstd::optional<ByEmail>() const;
    std::optional<ByPhone>& makestd::optional<ByPhone>();
    std::optional<ByMail>& makestd::optional<ByMail>();
    std::optional<ByEmail>& makestd::optional<ByEmail>();
    std::optional<ByPhone>& byPhone() { return data_.byPhone_; }
    const std::optional<ByPhone>& byPhone() const { return data_.byPhone_; }
    std::optional<ByMail>& byMail() { return data_.byMail_; }
    const std::optional<ByMail>& byMail() const { return data_.byMail_; }
    std::optional<ByEmail>& byEmail() { return data_.byEmail_; }
    const std::optional<ByEmail>& byEmail() const { return data_.byEmail_; }
    std::ostream& print(std::ostream& os) const;
};
class ContactBook
{
    std::vector<Contact> book_;
public:
    ContactBook() {}
    std::vector<Contact>& book() { return book_; }
    const std::vector<Contact>& book() const { return book_; }
    std::ostream& print(std::ostream& os) const;
};