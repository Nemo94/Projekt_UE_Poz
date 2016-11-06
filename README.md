# Projekt_UE_Poz
Pozycjonowanie


Wersja 1.0

Co zostalo zrobione:
 - powinna dzialac z akcelerometrem LIS3DH, ktory zamowilismy oraz magnetometrem HMC5883, a takze wyswietlac dane na konsoli ustawionej na 9600, wymaga podlaczenia odpowiednich pinow do zewnetrznego konwertera UART -> USB
 - napisalem sterownik, ktory steruje 4 ukladami DRV8825 StepStick i tym samym 4 silnikami krokowymi - obsluga wlaczenia, wylaczenia, kierunku i zadania liczby krokow

Nastepne kroki do zrobienia w projekcie:
- sprawdzenie obslugi zewnetrznego akceleroemetru pod SPI
- sprawdzenie obslugi zewnetrznego magnerometru pod I2C
- sprawdzenie i w przypadku niedzialania, poprawienie algorytmu umozliwiajacego wyswietlanie mierzonych danych na konsoli przez UART
- sprawdzenie sterowania silnikami
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

Sterowniki silnikow krokowych:

PE4, PE7, PE10, PE13 - EN
PE5, PE8, PE11, PE14 - DIR
PE6, PE9, PE12, PE15 - STEP
