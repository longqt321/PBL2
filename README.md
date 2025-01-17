# PBL2

•	Folder code nhận được sau khi giải nén không được đặt ở Desktop.
•	Việc chỉnh sửa kích thước màn hình console trong quá trình sử dụng ứng dụng sẽ xảy ra lỗi.
•	Chương trình sử dụng compiler GNU GCC với phiên bản ngôn ngữ là C++20. Nếu như sử dụng compiler khác hoặc phiên bản ngôn ngữ thấp hơn thì có khả năng sẽ xảy ra lỗi.
•	Phiên bản gcc và g++ được sử dụng trong quá trình xây dựng chương trình là 14.2.0
•	Phải kết thúc chương trình bằng cách nhấn nút “Exit” ở màn hình đăng nhập. Nếu kết thúc bằng cách khác có thể tiềm ẩn lỗi.
•	Chương trình yêu cầu thiết bị phải cài đặt cmake để có thể chạy.
Hướng dẫn cách chạy chương trình.
Chạy file build.bat để build chương trình, quá trình build có thể mất từ 30 giây đến 1 phút. 
Sau khi build thành công sẽ hiện ra dòng thông báo “Open folder build and run Salon_Management.exe (screen size 120*40)”.
 
Hình 1. Nội dung file build.bat
Vào folder “build” và mở file Salon_Management.exe để khởi động chương trình.
Nếu như màn hình chương trình bị lỗi nhảy loạn xạ (lỗi do kích thước console mặc định trên thiết bị không đúng so với thiết kế của chương trình). Xin hãy chuột phải vào thanh phía trên của command line hoặc powershell và chọn settings -> launch size -> Set kích thước column thành 120 và rows thành 40.
Xem ảnh minh họa ở dưới.
   

Nếu như không thể chạy file build.bat, có thể chạy lần lượt các câu lệnh dưới đây
mkdir build
cd build
cmake ../ -G "MinGW Makefiles"
cmake --build .
Tài khoản sử dụng cho chương trình
Tài khoản Admin.
Username: admin
Password: pass
Tài khoản Customer.
Username: longqt (tài khoản đã có sẵn dữ liệu), giang, minh123, customer (Tài khoản trắng, không có dữ liệu được chuẩn bị trước).
Password: pass (Tất cả các tài khoản Customer ở trên đều dùng chung 1 password là pass)
Tài khoản Stylist.
Username: longsalon (Tài khoản đã có sẵn dữ liệu), giangsalon (Tài khoản đã có sẵn dữ liệu), namsalon, stylist (Tài khoản trắng, không có dữ liệu được chuẩn bị trước)
Password: 123salon (Tất cả các tài khoản Stylist ở trên đều dùng chung 1 password là 123salon)
Tài khoản stylist có thể được thêm mới bởi tài khoản admin.
Tài khoản customer có thể được tạo bởi người dùng.
