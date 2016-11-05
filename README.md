# Projekt_UE_Poz
Pozycjonowanie


Wersja 1.0

Co zostalo zrobione:
 - powinna dzialac z akcelerometrem LIS3DH, ktory zamowilismy oraz magnetometrem HMC5883, a takze wyswietlac dane na konsoli ustawionej na 9600, wymaga podlaczenia odpowiednich pinow do zewnetrznego konwertera UART -> USB

Nastepne kroki do zrobienia w projekcie:
- sprawdzenie obslugi zewnetrznego akceleroemetru pod SPI
- sprawdzenie obslugi zewnetrznego magnerometru pod I2C
- sprawdzenie i w przypadku niedzialania, poprawienie algorytmu umozliwiajacego wyswietlanie mierzonych danych na konsoli przez UART
- obsluga silnikow za pomoca mostkow albo sterownikow silnikow krokowych?
- przetowrzenie danych na liczbe krokow?


PODLACZENIE:

Do konwertera UART - USB
 PB11------> USART3_RX

 PD8------> USART3_TX 
 
 GND ---> GND
 
 Do akcelerometru:
 PD0 - INT1
 
 PD1 - INT2
 
 PD3 - CS
 
 PC2 - MISO
 
 PB15 - MOSI
 
 PB13 - SCK
 
 Do magnetometru:
 PB6------> SCL
 
 PB9------> SDA 
