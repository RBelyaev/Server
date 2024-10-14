#include "Client.h"

void client::send_file(boost::asio::ip::tcp::socket &socket, std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удается открыть файл для отправки");
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(buffer), error);

    if (error) {
        throw boost::system::system_error(error);  // Обработка ошибок при отправке
    }
}


void client::receive_file(boost::asio::ip::tcp::socket& socket, const std::string& path) {
    try {
        std::ofstream output(path, std::ios::binary);
        if (!output) {
            throw std::runtime_error("Не удается открыть файл для записи");
        }

        std::vector<char> buffer(1024);  // Буфер для хранения данных
        boost::system::error_code error;
        size_t len;

        // Цикл для получения данных от сервера
        while ((len = socket.read_some(boost::asio::buffer(buffer), error)) > 0) {
            output.write(buffer.data(), len);  // Записываем полученные данные в файл
            if (error == boost::asio::error::eof) {
                std::cout << "Передача файла завершена." << std::endl;
                break;  // Конец передачи
            } else if (error) {
                throw boost::system::system_error(error);  // Обработка ошибок
            }
        }

        output.close();  // Закрываем файл
        std::cout << "Файл успешно получен и сохранен как: " << path << std::endl;
    } 
    catch (std::exception& e) {
        std::cerr << "Ошибка при получении файла: " << e.what() << std::endl;
    }
}




void client::run()
{
    try
    {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);
    
    auto endpoints = resolver.resolve("127.0.0.1", "8080");
    boost::asio::connect(socket, endpoints);


    std::string path = "examples/image.bmp";

    send_file(socket, path);
    std::cout << "Файл успешно отправлен!" << std::endl;


    path = "examples/image1.bmp";
    receive_file(socket, path);
    std::cout << "Файл успешно получен!" << std::endl;

    // Корректное завершение работы с сокетом
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket.close();
    }
    catch (std::exception& e) 
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    
}
