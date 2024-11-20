//
// Created by LONG on 11/8/2024.
//

#include "Stylist.h"
#include "Customer.h"
#include "Appointment.h"
#include "Database.h"
#include "Member.h"

Database<Customer>& dbCustomer = Database<Customer>::Connect(CUSTOMERS_FILE);

Stylist::Stylist(const string &ID, const string &firstName, const string &lastName,
    const bool &gender, const int &age, const string &phoneNumber, const string &username,
    const string &password)
    : Member(ID,firstName,lastName,gender,age,phoneNumber,username,password) {
}

Stylist::~Stylist() {

}

void Stylist::ViewScheduleByDay(const Datetime& dt) const {
    cout << "Schedule on: " << dt.GetDay() << "/" << dt.GetMonth() << "/" << dt.GetYear() << '\n';
    bool hasAppointments = false;

    for (const auto& appointment : schedule) {
        if (appointment.GetStylistID() != this->ID) continue;

        if (appointment.GetStartTime().GetDay() == dt.GetDay() &&
            appointment.GetStartTime().GetMonth() == dt.GetMonth() &&
            appointment.GetStartTime().GetYear() == dt.GetYear()) {
            if (!dbCustomer.IsExist("ID", appointment.GetCustomerID())) continue;
            hasAppointments = true;
            Customer customer = dbCustomer.Get(appointment.GetCustomerID());
            cout << "Schedule with: " << customer.GetFullName() << " (" << customer.GetID() << ")\n";
            cout << "Time: " << appointment.GetStartTime().GetHour() << ":" << appointment.GetStartTime().GetMinute() << "\n";
        }
    }
    if(!hasAppointments) {
        cout << "No Appointment!\n";
    }
}

void Stylist::ViewSchedule() const {
    // Tam thoi khach hang chua chon tho, nen tho chua co lich
}



bool Stylist::Login(const string& username,const string& password) const {
    Database<Stylist>& db = Database<Stylist>::Connect(STYLISTS_FILE);
    if (db.IsExist("username",username)&&db.IsExist("password",password)) {
        return true;
    }
    return false;
}

bool Stylist::Logout() const {
    return true;
}

ostream& operator<<(ostream& os,const Stylist& obj) {
    os << obj.ID << ' ' << Replace(obj.firstName,' ','-')  << ' ' << Replace(obj.lastName,' ','-') << ' '
        << obj.username << ' ' << obj.gender << ' '
        << obj.age << ' ' << obj.phoneNumber;
    return os;
}

istream& operator>>(istream& is,Stylist& obj) {
    is >> obj.ID >> obj.firstName >> obj.lastName >> obj.username >> obj.gender >> obj.age >> obj.phoneNumber;
    Database<Appointment>& db = Database<Appointment>::Connect(APPOINTMENTS_FILE);
    vector<Appointment> vt = db.Query("stylistID",obj.GetID()).GetResults();
    for (const Appointment& apt : vt) {
        obj.schedule.push_back(apt);
    }
    return is;
}

