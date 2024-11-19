#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include <array>
#include "ui.h"

using namespace ftxui;


void screenCustomer()
{
    // test stuff
    std::string name = callGetCurrentUserName();
    std::ofstream log;
    log.open("log.txt");

    // customer screen
    auto screen = ScreenInteractive::FixedSize(120, 30);
    int exit = 0;

    // Navigation tab buttons
    int selectedTab = 0;
    ButtonOption buttonOptionTab;
    buttonOptionTab.transform = [](const EntryState& s) {
        auto element = text(s.label) | center ;
        if (s.focused) {
            element |= inverted;
        }
        return element;
    };
    Component buttonHome = Button("Home", [&] {selectedTab = 0;}, buttonOptionTab);
    Component buttonAppointment = Button("Appointment", [&] {selectedTab = 1;}, buttonOptionTab);
    Component buttonHistory = Button("History", [&] {selectedTab = 2;}, buttonOptionTab);
    Component buttonProfile = Button("Profile", [&] {selectedTab = 3;}, buttonOptionTab);
    Component buttonLogout = Button("Logout", [&] {exit = 1;}, buttonOptionTab);
    Component containerButtons = Container::Vertical({
        buttonHome,
        buttonAppointment,
        buttonHistory,
        buttonProfile,
        buttonLogout,
    });
    Component rendererButtons = Renderer(containerButtons, [&] {
        return vbox({
            text("Welcome " + name) | hcenter,
            filler() | size(HEIGHT, EQUAL, 2),
            separator(),
            buttonHome->Render(),
            separator(),
            buttonAppointment->Render(),
            separator(),
            buttonHistory->Render(),
            separator(),
            buttonProfile->Render(),
            separator(),
            filler(),
            separator(),
            buttonLogout->Render()
        }) | borderRounded | size(WIDTH, EQUAL, 30);
    });

    // Home tab
    Component tabHome = Renderer([&] {
        return text("Home") | center ;
    });

    // Appointment tab
    // error handle
    std::string errorDateTime = "";
    std::string errorServicesEmpty = "";
    auto errorHandleAppointment = [&] (int errorCode) {
        switch (errorCode)
        {
            case ERROR_CODE::CREATE_APPOINTMENT_BUSY_TIME:
                errorDateTime = "The time you selected is already taken. Please choose another time.";
                break;
            case ERROR_CODE::CREATE_APPOINTMENT_CLOSED_TIME:
                errorDateTime = "The time you selected is outside of our working hours. Please choose another time.";
                break;
            case ERROR_CODE::CREATE_APPOINTMENT_SERVICES_EMPTY:
                errorServicesEmpty = "Please select at least one service.";
                break;
            default:
                break;
        }
    };

    int tabComfirm = 0;
    
    // Services
    const int servicesCount = SERVICES_COUNT;
    std::array<bool, servicesCount> selectedServices;
    std::array<int, servicesCount> selectedStylist;
    std::vector<std::string> services = {
        "Cat toc",
        "Nhuom toc",
        "Tay toc",
        "Uon toc",
        "Phuc hoi toc",
        "Tao kieu",
        "Goi dau"
    };
    std::vector<std::string> stylists[servicesCount];
    int selectedCheckboxService = 0;
    Component containerServices = Container::Vertical({}, &selectedCheckboxService);
    for (int i = 0; i < servicesCount; ++i) {
        selectedServices[i] = false;
        stylists[i] = callGetStylist(i);
        selectedStylist[i] = 0;
        Component newBox = Container::Horizontal({
                Checkbox(services[i], &selectedServices[i]) | size(WIDTH, EQUAL, 20),
                Dropdown({
                    .radiobox = {.entries = &(stylists[i]), .selected = &(selectedStylist[i])},
                    .transform = [](bool open, Element checkbox, Element radiobox) {
                        if (open) {
                            const int maxHeight = 4;
                            return vbox({
                                checkbox | inverted,
                                radiobox | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, maxHeight),
                            });
                        }
                        return vbox({checkbox, filler()}) ;
                    },
                    }) | size(WIDTH, EQUAL, 20),
        });
        containerServices->Add(newBox);
    }

    // Date and time
    int selectedDay = 0;
    int selectedMonth = 0;
    int selectedYear = 0;
    int selectedHour = 0;
    int selectedMinute = 0;

    std::vector<std::string> days = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
    std::vector<std::string> months = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
    std::vector<std::string> years  = {"2021", "2022", "2023", "2024", "2025", "2026", "2027", "2028", "2029", "2030"};
    std::vector<std::string> hours = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"};
    std::vector<std::string> minutes = {"00", "15", "30", "45"};


    MenuOption menuOptionAll;
    menuOptionAll.direction = Direction::Up;
    menuOptionAll.entries_option.transform = [](const EntryState& s) {
        auto element = text(s.label) | center;
        if (s.focused) {
            element = element | inverted;
        }
        return element;
    };

    Component menuDay = Menu(&days, &selectedDay, menuOptionAll);
    MenuOption menuOptionMonth = MenuOption(menuOptionAll);
    menuOptionMonth.on_change = ([&] () {
        int curYear = 2021 + selectedYear;
        int curMonth = 1 + selectedMonth;
        int curDay = 1 + selectedDay;
        bool isLeapYear = (curYear % 4 == 0 && curYear % 100 != 0) || (curYear % 400 == 0);
        int maxDay;  
        if (curMonth == 2) {
            maxDay = isLeapYear ? 29 : 28;
        }
        else if (curMonth == 4 || curMonth == 6 || curMonth == 9 || curMonth == 11) {
            maxDay = 30;
        }
        else {
            maxDay = 31;
        }
        if (curDay > maxDay) {
            selectedDay = maxDay - 1;
        }
        days.clear();
        for (int i = 1; i <= maxDay; ++i) {
            days.push_back(std::to_string(i).size() == 1 ? "0" + std::to_string(i) : std::to_string(i));
        }
    });

    Component menuMonth = Menu(&months, &selectedMonth, menuOptionMonth);
    Component menuYear = Menu(&years, &selectedYear, menuOptionAll);
    Component menuHour = Menu(&hours, &selectedHour, menuOptionAll);
    Component menuMinute = Menu(&minutes, &selectedMinute, menuOptionAll);

    Component containerDate = Container::Horizontal({
        menuDay,
        menuMonth,
        menuYear,
    });
    Component containerTime = Container::Horizontal({
        menuHour,
        menuMinute,
    });

    Component rendererDate = Renderer(containerDate, [&] {
        return hbox({
            text("Date: ") | size(WIDTH, EQUAL, 10),
            menuDay->Render() | size(WIDTH, EQUAL, 2) | frame | size(HEIGHT, EQUAL, 1),
            text("/") | size(WIDTH, EQUAL, 1),
            menuMonth->Render() | size(WIDTH, EQUAL, 2) | frame | size(HEIGHT, EQUAL, 1),
            text("/") | size(WIDTH, EQUAL, 1),
            menuYear->Render() | size(WIDTH, EQUAL, 4) | frame | size(HEIGHT, EQUAL, 1),
        });
    });
    Component rendererTime = Renderer(containerTime, [&] {
        return hbox({
            text("Time: ") | size(WIDTH, EQUAL, 10),
            menuHour->Render() | size(WIDTH, EQUAL, 2) | frame | size(HEIGHT, EQUAL, 1),
            text(":") | size(WIDTH, EQUAL, 1),
            menuMinute->Render() | size(WIDTH, EQUAL, 2) | frame | size(HEIGHT, EQUAL, 1),
        });
    });

    // Catch event
    containerServices |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab;
        if (check && selectedCheckboxService < 6) {
            selectedCheckboxService++;
        } else if (check) {
            rendererDate->TakeFocus();
        }
        return check;
    });
    containerServices |= CatchEvent([&] (Event event) {
        bool check = event == Event::TabReverse;
        if (check && selectedCheckboxService > 0) {
            selectedCheckboxService--;
        }
        return check;
    });
    rendererDate |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab || event == Event::Return;
        if (check) {
            containerTime->TakeFocus();
        }
        return check;
    });
    rendererDate |= CatchEvent([&] (Event event) {
        bool check = event == Event::TabReverse;
        if (check) {
            containerServices->TakeFocus();
        }
        return check;
    });
    rendererTime |= CatchEvent([&] (Event event) {
        bool check = event == Event::TabReverse;
        if (check) {
            rendererDate->TakeFocus();
        }
        return check;
    });

    // Requirement
    std::string requirement = "";
    InputOption inputOptionRequirement;
    inputOptionRequirement.transform = [](InputState state) {
        state.element |= color(Color::White);

        if (state.is_placeholder) {
            state.element |= dim;
        }

        if (state.focused) {
            state.element |= inverted;
        } else if (state.hovered) {
        state.element |= bgcolor(Color::GrayDark);
    }
    state.element |= borderRounded;
    state.element |= size(WIDTH, EQUAL, 40);
    state.element |= size(HEIGHT, EQUAL, 8);
    return state.element;
    };
    Component inputRequirement = Input(&requirement, inputOptionRequirement);

    // event
    rendererTime |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab || event == Event::Return;
        if (check) {
            inputRequirement->TakeFocus();
        }
        return check;
    });

    // Button next
    ButtonOption buttonOptionNext;
    buttonOptionNext.transform = [](const EntryState& s) {
        auto element = text(s.label) | center | borderRounded;
        if (s.focused) {
            element |= inverted;
        }
        return element;
    };
    
    std::string newAppointmentId;
    Component buttonNext = Button("Next", [&] {
        errorDateTime = "";
        errorServicesEmpty = "";
        try {
            callCreateNewAppointment(selectedServices, selectedStylist, selectedDay, selectedMonth, selectedYear, selectedHour, selectedMinute, requirement);
            newAppointmentId = callGetNewAppointmentId();
            tabComfirm = 1;
        }
        catch (int errorCode) {
            errorHandleAppointment(errorCode);
        }
    }, buttonOptionNext) | size(WIDTH, EQUAL, 8);
    buttonNext |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab;
        return check;
    });

    Component tabAppointmentInput = Renderer( 
        Container::Vertical({
                containerServices, 
                rendererDate,
                rendererTime,
                inputRequirement,
                buttonNext
        }) ,[&] {
            auto textError = [&](std::string error) {
                if (error.size() == 0) {
                    return text("") | size(HEIGHT, EQUAL, 0);
                }
                return paragraph(error) | align_right | color(Color::Red);
            };

            return vbox({
                hbox({
                    text("Services") | size(WIDTH, EQUAL, 20),
                    text("Stylist") | size(WIDTH, EQUAL, 20),
                }),
                containerServices->Render(),
                textError(errorServicesEmpty),
                text("Select date and time"),
                rendererDate->Render(),
                rendererTime->Render(),
                textError(errorDateTime),
                text("Reqirements"),
                inputRequirement->Render(),
                buttonNext->Render()| align_right,
            }) | size(WIDTH, EQUAL, 40);
    });

    // Comfirm appointment
    auto resetAppointment = [&] {
        for (int i = 0; i < servicesCount; ++i) {
            selectedServices[i] = false;
            selectedStylist[i] = 0;
        }
        selectedCheckboxService = 0;
        selectedDay = 0;
        selectedMonth = 0;
        selectedYear = 0;
        selectedHour = 0;
        selectedMinute = 0;
        requirement = "";
        newAppointmentId = "";
    };
    int selectedButtonsAppointment = 1;

    ButtonOption buttonOptionConfirm(buttonOptionNext);
    Component buttonAppointmentConfirm = Button("Comfirm", [&] {
        tabComfirm = 0;
        resetAppointment();
        callAddNewAppointment();
    }, buttonOptionConfirm);
    Component buttonAppointmentCancel = Button("Cancel", [&] {
        tabComfirm = 0;
        selectedButtonsAppointment = 1;
    }, buttonOptionConfirm);
    Component tabAppointmentConfirm = Renderer(Container::Horizontal({
        buttonAppointmentCancel,
        buttonAppointmentConfirm
    }, &selectedButtonsAppointment) ,[&] {
        auto textService = [&](int i) {
            if (!selectedServices[i]) {
                return text("") | size(HEIGHT, EQUAL, 0);
            }
            return hbox({
                text(services[i]) | size(WIDTH, EQUAL, 20),
                text(stylists[i][selectedStylist[i]]) | size(WIDTH, EQUAL, 20),
            });
        };
        Elements textServiceList;
        for (int i = 0; i < servicesCount; ++i) {
            textServiceList.push_back(textService(i));
        }

        return vbox({
            text("Comfirm appointment") | center,
            separator(),
            text("Appointment ID") | bold,
            text(newAppointmentId),
            filler() | size(HEIGHT, EQUAL, 1),
            hbox({
                text("Services") | size(WIDTH, EQUAL, 20),
                text("Stylist") | size(WIDTH, EQUAL, 20),
            }) | bold,
            vbox(textServiceList),
            filler() | size(HEIGHT, EQUAL, 1),
            text("Date and time") | size(WIDTH, EQUAL, 40) | bold,
            text("Date: " + days[selectedDay] + "/" + months[selectedMonth] + "/" + years[selectedYear]) | size(WIDTH, EQUAL, 40),
            text("Time: " + hours[selectedHour] + ":" + minutes[selectedMinute]) | size(WIDTH, EQUAL, 40),
            filler() | size(HEIGHT, EQUAL, 1),
            text("Requirement: ") | size(WIDTH, EQUAL, 40) | bold,
            paragraph(requirement) | size(WIDTH, EQUAL, 40),
            filler(),
            separator(),
            hbox({
                buttonAppointmentCancel->Render() | size(WIDTH, EQUAL, 10),
                filler() | size(WIDTH, EQUAL, 5),
                buttonAppointmentConfirm->Render() | size(WIDTH, EQUAL, 10),
            })| hcenter,
        }) | center | borderRounded | size(WIDTH, EQUAL, 40);
    });
    
    Component tabAppointment = Renderer(
        Container::Tab({
            tabAppointmentInput,
            tabAppointmentConfirm
        }, &tabComfirm), [&] {
        auto document = hbox({
            filler() | size(WIDTH, EQUAL, 2),
            tabAppointmentInput->Render(),
        });
        if (tabComfirm == 1) {
            document = dbox({
                hbox({
                filler() | size(WIDTH, EQUAL, 2),
                tabAppointmentInput->Render(),
                }),
                tabAppointmentConfirm->Render() | clear_under | align_right,
            });
        }
        return document;
    });

    // History tab
    Component tabHistory = Renderer([&] {
        return text("History") | center ;
    });

    // Profile tab
    std::string firstname;
    std::string lastname;
    std::string username;
    std::string oldPassword;
    std::string newPassword;
    std::string confirmpassword;
    std::string phonenumber;
    std::string age;
    int gender = 0;

    auto resetName = [&] {
        callGetCurrentUserName(firstname, lastname);
    };
    auto resetPassword = [&] {
        oldPassword = "";
        newPassword = "";
        confirmpassword = "";
    };
    auto resetPersonInfo = [&] {
        callGetCurrentUserPersonInfo(phonenumber, age, gender);
    };
    auto resetProfile = [&] {
        resetName();
        resetPassword();
        resetPersonInfo();
    };
    username = callGetCurrentUserUsername();
    resetProfile();

    const int PROFILE_ALIGN_WIDTH = 50;

    InputOption inputOptionProfile = InputOption::Default();
    inputOptionProfile.placeholder = "";
    inputOptionProfile.multiline = false;

    Component inputFirstname = Input(&firstname, "First Name", inputOptionProfile);
    Component inputLastname = Input(&lastname, "Last Name", inputOptionProfile);

    InputOption inputOptionPassword(inputOptionProfile);
    inputOptionPassword.password = true;

    Component inputOldpassword = Input(&oldPassword, "Old Password", inputOptionPassword);
    Component inputNewpassword = Input(&newPassword, "New Password", inputOptionPassword);
    Component inputConfirmpassword = Input(&confirmpassword, "Confirm Password", inputOptionPassword);

    Component inputPhonenumber = Input(&phonenumber, "Phone Number", inputOptionProfile);
    Component inputAge = Input(&age, "Age", inputOptionProfile);

    ComponentDecorator eventNumberOnly = CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });
    inputPhonenumber |= eventNumberOnly;
    inputPhonenumber |=  CatchEvent([&](Event event) {
        return event.is_character() && phonenumber.size() >= 11;
    });
    inputAge |= eventNumberOnly;
    inputAge |= CatchEvent([&](Event event) {
        return event.is_character() && age.size() >= 3;
    });
    ComponentDecorator eventAlphabetOnly = CatchEvent([&](Event event) {
        return event.is_character() && 
            !std::isalpha(event.character()[0]) && 
            !std::isspace(event.character()[0]);
    });
    inputFirstname |= eventAlphabetOnly;
    inputLastname  |= eventAlphabetOnly;

    std::vector <std::string> genders = {"Male", "Female"};
    Component radioboxGender = Radiobox(&genders, &gender);

    std::string errorUpdateProfileFirstnameEmpty = "";
    std::string errorUpdateProfileLastnameEmpty  = "";
    Component buttonNameUpdate = Button("Update", [&] {
        errorUpdateProfileFirstnameEmpty = "";
        errorUpdateProfileLastnameEmpty = "";
        try {
            callUpdateCurrentUserName(firstname, lastname);
        }
        catch (int errorCode) {
            switch (errorCode)
            {
                case ERROR_CODE::UPDATE_PROFILE_NAME_EMPTY:
                    errorUpdateProfileFirstnameEmpty = "First name is empty";
                    errorUpdateProfileLastnameEmpty = "Last name is empty";
                    break;
                case ERROR_CODE::UPDATE_PROFILE_FIRSTNAME_EMPTY:
                    errorUpdateProfileFirstnameEmpty = "First name is empty";
                    break;
                case ERROR_CODE::UPDATE_PROFILE_LASTNAME_EMPTY:
                    errorUpdateProfileLastnameEmpty = "Last name is empty";
                    break;
                default:
                    break;
            }
        }
    }, buttonOptionTab);
    Component buttonNameReset = Button("Reset", [&] {
        errorUpdateProfileFirstnameEmpty = "";
        errorUpdateProfileLastnameEmpty = "";
        resetName();
    }, buttonOptionTab);

    std::string errorUpdateProfilePassword = "";
    Component buttonPasswordUpdate = Button("Update", [&] {
        errorUpdateProfilePassword = "";
        try {
            callUpdateCurrentPassword(oldPassword, newPassword, confirmpassword);
        }
        catch (int errorCode) {
            switch (errorCode)
            {
                case ERROR_CODE::UPDATE_PROFILE_INVALID_PASSWORD:
                    errorUpdateProfilePassword = "Invalid old password";
                    break;
                case ERROR_CODE::UPDATE_PROFILE_PASSWORD_NOT_MATCH:
                    errorUpdateProfilePassword = "Password not match";
                    break;
                default:
                    break;
            }
        }
    }, buttonOptionTab);
    Component buttonPasswordReset = Button("Reset", [&] {
        errorUpdateProfilePassword = "";
        resetPassword();
    }, buttonOptionTab);

    std::string errorUpdateProfilePersoninfoEmpty = "";
    Component buttonPersoninfoUpdate = Button("Update", [&] {
        errorUpdateProfilePersoninfoEmpty = "";
        try {
            callUpdateCurrentPersonInfo(phonenumber, age, gender);
        }
        catch (int errorCode) {
            switch (errorCode)
            {
                case ERROR_CODE::UPDATE_PROFILE_PERSONINFO_EMPTY:
                    errorUpdateProfilePersoninfoEmpty = "Phone number or age is empty";
                    break;
                default:
                    break;
            }
        }
    }, buttonOptionTab);
    
    Component buttonPersoninfoReset = Button("Reset", [&] {
        errorUpdateProfilePersoninfoEmpty = "";
        resetPersonInfo();
    }, buttonOptionTab);

    Component containerButtonsName = Container::Horizontal({
        buttonNameReset,
        buttonNameUpdate,
    });

    Component containerButtonsPassword = Container::Horizontal({
        buttonPasswordReset,
        buttonPasswordUpdate,
    });

    Component containerButtonsPersoninfo = Container::Horizontal({
        buttonPersoninfoReset,
        buttonPersoninfoUpdate,
    });

    radioboxGender |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab;
        if (check) {
            buttonPersoninfoReset->TakeFocus();
        }
        return check;
    });
    radioboxGender |= CatchEvent([&] (Event event) {
        bool check = event == Event::TabReverse;
        if (check) {
            inputAge->TakeFocus();
        }
        return check;
    });
    containerButtonsName |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab;
        if (check) {
            inputOldpassword->TakeFocus();
        }
        return check;
    });

    containerButtonsPassword |= CatchEvent([&] (Event event) {
        bool check = event == Event::Tab;
        if (check) {
            inputPhonenumber->TakeFocus();
        }
        return check;
    });

    auto textError = [&](std::string error) {
        if (error.size() == 0) {
            return text("") | size(HEIGHT, EQUAL, 0);
        }
        return paragraph(error) | align_right | color(Color::Red);
    };
    Component containerUpdateName = Renderer(Container::Vertical({
        inputFirstname,
        inputLastname,
        containerButtonsName
    }), [&] {

        return vbox({
            hbox({text("First Name      : "), inputFirstname->Render()}),
            textError(errorUpdateProfileFirstnameEmpty),
            hbox({text("Last Name       : "), inputLastname->Render()}),
            textError(errorUpdateProfileLastnameEmpty),
            separator(),
            hbox({
                buttonNameReset->Render() | size(WIDTH, EQUAL, 10),
                buttonNameUpdate->Render() | size(WIDTH, EQUAL, 10),
            }) | hcenter,
        }) | borderRounded | size(WIDTH, EQUAL, PROFILE_ALIGN_WIDTH);
    });

    Component containerUpdatePassword = Renderer(Container::Vertical({
        inputOldpassword,
        inputNewpassword,
        inputConfirmpassword,
        containerButtonsPassword,
    }), [&] {
        return vbox({
            hbox({text("Old Password    : "), inputOldpassword->Render()}),
            hbox({text("New Password    : "), inputNewpassword->Render()}),
            hbox({text("Confirm Password: "), inputConfirmpassword->Render()}),
            textError(errorUpdateProfilePassword),
            separator(),
            hbox({
                buttonPasswordReset->Render() | size(WIDTH, EQUAL, 10),
                buttonPasswordUpdate->Render() | size(WIDTH, EQUAL, 10),
            }) | hcenter,
        }) | borderRounded | size(WIDTH, EQUAL, PROFILE_ALIGN_WIDTH);
    });

    Component containerUpdatePersoninfo = Renderer(Container::Vertical({
        inputPhonenumber,
        inputAge,
        radioboxGender,
        containerButtonsPersoninfo,
    }), [&] {
        return vbox({
            hbox({text("Phone Number    : "), inputPhonenumber->Render()}),
            hbox({text("Age             : "), inputAge->Render()}),
            hbox({text("Gender          : "), radioboxGender->Render()}),
            textError(errorUpdateProfilePersoninfoEmpty),
            separator(),
            hbox({
                buttonPersoninfoReset->Render() | size(WIDTH, EQUAL, 10),
                buttonPersoninfoUpdate->Render() | size(WIDTH, EQUAL, 10),
            }) | hcenter,
        }) | borderRounded | size(WIDTH, EQUAL, PROFILE_ALIGN_WIDTH);
    });

    Component tabProfile = Renderer(Container::Vertical({
        containerUpdateName,
        containerUpdatePassword,
        containerUpdatePersoninfo,
    }), [&] {
        return vbox({
            text("Username") | bold,
            text(username),
            text("Name") | bold,
            containerUpdateName->Render(),
            text("Password") | bold,
            containerUpdatePassword->Render(),
            text("Personal Information") | bold,
            containerUpdatePersoninfo->Render(),
        }) | size(WIDTH, EQUAL, PROFILE_ALIGN_WIDTH) | center;
    });

    
    // tab container
    Component containerTabs = Container::Tab({
        tabHome,
        tabAppointment,
        tabHistory,
        tabProfile,
    }, &selectedTab) | borderRounded | size(WIDTH, EQUAL, 90);

    // All
    Component containerAll = Container::Horizontal({
        rendererButtons,
        containerTabs,
    });

    // exit dialog
    Component buttonYes = Button("Yes", [&] {
        exit = 0;
        currentScreen = screenWelcome;
        screen.Exit();
    }, buttonOptionTab);
    Component buttonNo = Button("No", [&] {
        exit = 0; 
    }, buttonOptionTab);
    Component containerButtonsExit = Container::Horizontal({
        buttonNo,
        buttonYes,
    });
    Component DialogExit = Renderer([&] {
        return vbox({
            paragraphAlignCenter("Do you really want to logout?") | size(WIDTH, EQUAL, 21),
            separator(),
            hbox({
                buttonNo->Render() | size(WIDTH, EQUAL, 10),
                separator(),
                buttonYes->Render()| size(WIDTH, EQUAL, 10)
            }) 
        }) | borderRounded;
    });

    Component rendererAll = Renderer(Container::Tab({containerAll, containerButtonsExit}, &exit), [&] {
        auto document = containerAll->Render();
        if (exit == 1) {
            document = dbox({
                document,
                DialogExit->Render() | clear_under | center,
            });
        }
        return document;
    });
    screen.Loop(rendererAll);
}