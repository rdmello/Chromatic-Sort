
#include <iostream>
#include <Magick++.h>


int main() {
    std::cout << "Hello?" << std::endl;

    Magick::Image my_img("images/Lenna.png");
    my_img.write("images/Lenna2.png");
    my_img.display();

    return 0;
}
