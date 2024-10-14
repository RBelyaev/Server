#include "Server.h"

void server::accept_file(boost::asio::ip::tcp::socket& socket, std::string& path)
{
    std::vector<char> buffer(1024);
    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Не удается открыть файл для записи");
    }

    boost::system::error_code error;
    size_t len;
    while ((len = socket.read_some(boost::asio::buffer(buffer), error)) > 0) {
        output.write(buffer.data(), len);  // Записываем только фактически прочитанные данные
        if (error == boost::asio::error::eof) {
            break;  // Конец передачи
        } else if (error) {
            throw boost::system::system_error(error);  // Обработка других ошибок
        }
    }
    output.close();
}


void server::send_file(boost::asio::ip::tcp::socket& socket, const std::string& file_path) {
    // Открываем файл и отправляем данные
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удается открыть файл для отправки");
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(buffer), error);

    if (error) {
        throw boost::system::system_error(error);
    }
}


int server::run()
{
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));


        while(1)
        {
        boost::asio::ip::tcp::socket socket(io_context);

        // Ожидание подключения клиента
        acceptor.accept(socket);

        // Прием данных
        std::string path1 = "ans/new1.bmp";
        accept_file(socket, path1);

        send_file(socket, path1);

        // Корректное завершение работы с сокетом
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket.close();
        }
        
    } 
    catch (std::exception& e) 
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    //print();
    return 0;
}








        // std::cout << "Файл успешно получен!" << std::endl;

        // std::cout << "Файл успешно отправлен!" << std::endl;