//
// Created by LONG on 11/27/2024.
//

#include "Salon.h"

#include <Appointment.h>
#include <Database.h>


Salon& Salon::StartUp() {
    static Salon salon;
    return salon;
}

Salon::~Salon() {
    userID.clear();
}

std::string Salon::GetUserID() {
    return this->userID;
}

int Salon::GetUserRole() {
    return this->userRole;
}


void Salon::ensurePermission(const std::string& requiredRole) {
    if (userRole == 0) throw ERROR_CODE::SALON_SOME_FIELD_EMPTY;
    int role=0;
    switch(requiredRole) {
        case "customer":
            role = 1;
            break;
        case "stylist":
            role = 2;
            break;
        case "admin":
            role = 3;
            break;
    }
    if (userRole != role && userRole != 3)
        throw ERROR_CODE::SALON_ACCESS_DENIED;
}

bool Salon::Login(const std::string& username, const std::string& password) {
    if (username.empty() && password.empty())
        throw ERROR_CODE::LOGIN_USER_AND_PASS_EMPTY;
    if (username.empty())
        throw ERROR_CODE::LOGIN_USERNAME_EMPTY;
    if (password.empty())
        throw ERROR_CODE::LOGIN_PASSWORD_EMPTY;

    std::vector<Member> members = dbUser.Query("username",username).
                                        Query("password",password).
                                        GetResults();
    //for (auto x : members) x.Show();

    if (members.size()==1) {
        userID = members[0].GetID();
        userRole = dbUser.Get(userID).GetRole();
        return true;
    }
    return false;
}

void Salon::Logout() {
    userID.clear();
    userRole=0;
}

void Salon::Register(const std::string& firstName,const std::string& lastName,const std::string& username,const std::string& password,const std::string& confirmPassword,
                    const bool& gender,const std::string& phoneNumber,const int& role) {
    if (firstName.empty() || lastName.empty() || username.empty() || password.empty() || confirmPassword.empty() || phoneNumber.empty())
        throw ERROR_CODE::REGISTER_SOME_FIELD_EMPTY;
    if (password != confirmPassword)
        throw ERROR_CODE::REGISTER_PASSWORD_NOT_MATCH;
    if (dbUser.IsExist("username",username))
        throw ERROR_CODE::REGISTER_USERNAME_EXIST;
    Member instance("null",firstName,lastName,username,password,gender,phoneNumber,role);
    dbUser.Insert(instance);
}


void Salon::CreateAppointment(const std::string& customerID,const std::string& stylistID, const Datetime& dt, const std::vector<Service>& serviceList) {
    if(serviceList.empty()) throw ERROR_CODE::CREATE_APPOINTMENT_SERVICES_EMPTY;
    if(customerID.empty()) throw ERROR_CODE::CREATE_APPOINTMENT_CUSTOMER_EMPTY;

    ensurePermission("customer");

    Appointment instance("null",dt,customerID,stylistID,serviceList);
    dbAppointment.Insert(instance);
}

void Salon::DeleteAppointment(const std::string& appointmentID) {
    if (appointmentID.empty()) throw ERROR_CODE::SALON_SOME_FIELD_EMPTY;

    ensurePermission("admin");

    dbAppointment.Delete(appointmentID);
}

void Salon::ShowAppointment(const Datetime& dt) {
    ensurePermission("customer");
    dbAppointment.Query("customerID",userID).Query("startTime",Datetime::TimeToString(dt)).Show();
}

void Salon::ShowAllAppointment(const Datetime& dt) {
    ensurePermission("admin");
    dbAppointment.Query("startTime",Datetime::TimeToString(dt)).Show();
}
void Salon::ShowSchedule(const std::string& stylistID, const Datetime& dt) {

}
void Salon::ShowAllSchedule(const std::string& stylistID) {

}