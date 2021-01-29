#ifndef AES_H
#define AES_H
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <QObject>
#include <QDebug>
using namespace std;
class AES:public QObject
{
    Q_OBJECT
private:
    void encrypt_block(){
//        print_state();
        xorsi(0);
//        print_state();
        for(int i = 1; i < 10; ++i) {
            encsi(i);
//            print_state();
        }
        enclastsi();
//        print_state();
        std::cout << std::endl;
    }
    void decrypt_block(){
        xorsi(10);
        for(int i = 11; i < 20; ++i) {
            decsi(i);
        }
        declastsi();
    }
    void print_state() {
        string state = get_state();
        for(auto i : state) {
            std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)i << " ";
        }
        cout <<endl;
    }
    virtual string get_state() const = 0;
    virtual void set_state(char* block) = 0;
    virtual void set_key(const string & key) = 0;
    virtual void encsi(int round) = 0;
    virtual void enclastsi() = 0;
    virtual void decsi(int round) = 0;
    virtual void declastsi() = 0;
    virtual void xorsi(int key) = 0;
public:
    AES(QObject * parent = Q_NULLPTR):QObject(parent){}
    virtual ~AES() {};

    string encrypt(string plain_text){
        string result;
        if(plain_text.size() % 16)
            plain_text.append(16 - plain_text.size() % 16, 0xff);
        for(size_t i = 0; i < plain_text.size(); i += 16) {
            char *block = &plain_text[i];
            set_state(block);
            encrypt_block();
            result += get_state();
        }
        return result;
    }

    string decrypt(string cipher_text){
        string result;
        if(cipher_text.size() % 16)
            cipher_text.append(16 - cipher_text.size() % 16, 0xff);
        for(size_t i = 0; i < cipher_text.size(); i += 16) {
            char *block = &cipher_text[i];
            set_state(block);
            decrypt_block();
            result += get_state();
        }
        return result;
    }
public slots:
    void encrypt_text() const {
        qDebug()<<"Encrypt Text.";
    }
    void decrypt_text() const {
        qDebug()<<"Decrypt Text.";
    }
};
#endif // AES_H
