# Systemy Operacyjne Laboratoria 2025
## Autor: Gabriel Kania
## Ważne informacje
Plik README.md zawiera informacje jak budować i uruchamiać programy z laboratoriów z przedmiotu Systemy Operacyjne. Do każdego laboratorium przygotowałem krótki opis razem z ewentualnym objaśnieniem mojego rozwiązania. Pełne polecenia znajdują się w pliku `zadania.md`. Niektóre programy mogą być uruchomione na kilka sposobów - ręcznie lub za pomocą skryptu testującego (w niektórych przypadkach nie ma to znaczenia, bo skrypt testujący jest tylko wzbogacony o proste formatowanie w celach logowania do pliku). Środowisko Code zostało skonfigurowane z myślą o systemie Ubuntu - nie powinno mieć to większego znaczenie.
## Makefile
Plik makefile został wzbogacony o automatyczne testy wszystkich programów wywoływany komendą
```bash
make labs
```
oprócz tego możliwe jest zbudowanie i przetestowanie programów z danego laboratorium poprzez
```bash
make lab<nr_lab>
```
gdzie nr_lab oznacza 2-cyfrowy numer laboratorium (np. 01, 03, 10, itp.). Automatyczne testy zapisują output do pliku `log.txt`.

Dostępne są również funkcje czyszczące oraz budujące wszystkie dostępne programy
```bash
# budowa wszystkich programów
make all
# czyszczenie wykonywalnych plików binarnych programów i plików tymczsowych
make clean
# czyszczenie do stanu początkowego
make cleaner 
```
Komendy budujące poszczególne programy razem z instrukcją uruchamiania dostępne są w następnej sekcji.

## Laboratoria
### Lab 01: Podstawy Makefile
Na tym laboratorium mieliśmy wykonać prosty licznik i zbudować go przy pomocy **make**.
```bash
make countdown
./countdown
```
lub
```bash
make countdown
./test/countdown_test.sh
```
### Lab 02: Biblioteki
Na tym laboratorium uczyliśmy się budować oraz ładować do naszych programów **biblioteki statyczne, współdzielone** oraz **dynamiczne**. Napisaliśmy bibliotekę oraz 3 wersje klientów, które ładowały różne wersje naszej biblioteki. Do implementacji mieliśmy algorytm sprawdzający **zbieżność Collatz'a**.
#### Biblioteka statyczna
```bash
make client_static
./client_static
```
#### Biblioteka współdzielona
```bash
make client_shared
./client_shared
```
#### Biblioteka dynamiczna
```bash
make client_dynamic
./client_dynamic
```
Istnieje możliwość zbudowania i przetestowania bibliotek jedną komendą
```bash
make collatz
./test/collatz_test.sh
```
### Lab 03: Operacje na plikach (pliki lustrzane)
Na tym laboratorium uczyliśmy się obsługi plików. Za zadanie mieliśmy odwrócić pliki dwukrotnie i sprawdzić czy zachowały **integralność** za pomocą prostej **sumy kontrolnej** oraz **hasha MD5**.
Prosta suma kontrolna realizowana jest w programie `flipper.c`, natomiast sprawdzanie MD5 wykonuje osobny skrypt `md5checker.sh`, który jest automatycznie wywoływany na końcu działania programu flipper. 

Aby zachować integralność wszystkich plików, nie tylko tekstowych, zastosowałem podejście z odczytywaniem plików w trybie **binarnym**.

Pliki odwrócone 1 raz znajdują się w folderze `reversed`, a odwrócone 2 razy (przywrócone) w folderze `restored`.

```bash
make flipper
./flipper
```
lub
```bash
make flipper
./test/flipper_test.sh
```

### Lab 04: Procesy
Na tym laboratorium ćwiczyliśmy tworzenie i zarządzanie procesami w systemie Linux. Poznaliśmy funkcje takie jak `fork()`, `wait()`, `waitpid()` oraz `execl()`. Celem było zrozumienie różnic między procesem rodzicem a potomnym, przekazywania argumentów oraz synchronizacji zakończenia procesów.

#### Programy:
- `proc_1.c` – Tworzy zadaną liczbę procesów potomnych, każdy wypisuje swój PID oraz PID rodzica.
- `proc_2.c` – Pokazuje różnice między zmiennymi lokalnymi i globalnymi w procesie rodzica i potomnym oraz demonstruje użycie `execl()` do uruchomienia programu `/bin/ls` w procesie potomnym.

#### Budowanie i uruchamianie:
```bash
make proc_1
./proc_1 <liczba_procesów>
```
lub
```bash
make proc_1
./test/proc_test.sh
```

```bash
make proc_2
./proc_2 <ścieżka_katalogu>
```

#### Przykładowe użycie:
```bash
./proc_1 5
./proc_2 .
```

Skrypt testujący `proc_test.sh` automatycznie uruchamia oba programy i sprawdza poprawność działania.

### Lab 05: Sygnały
Na tym laboratorium poznaliśmy mechanizmy obsługi sygnałów w systemie Linux. Ćwiczyliśmy zarówno reakcje na sygnały (ignorowanie, obsługa własnym handlerem, maskowanie), jak i komunikację między procesami za pomocą sygnałów. Zaimplementowaliśmy dwa programy: `sender.c` (wysyłający sygnały) oraz `catcher.c` (odbierający sygnały i potwierdzający odbiór).

#### Programy:
- `sig_1.c` – Demonstruje różne reakcje na sygnał SIGUSR1 w zależności od argumentu wywołania (`none`, `ignore`, `handler`, `mask`).
- `catcher.c` – Odbiera sygnały SIGUSR1 od `sendera`, obsługuje różne tryby pracy (np. ignorowanie Ctrl+C, wypisywanie licznika, zakończenie pracy).
- `sender.c` – Wysyła sygnały SIGUSR1 do `catchera` wraz z trybem pracy, oczekuje potwierdzenia odbioru.

#### Budowanie i uruchamianie:
```bash
make sig_1
./sig_1 <none|ignore|handler|mask>
```
```bash
make catcher
make sender
./catcher
./sender <PID_catchera> <tryb>
```
lub
```bash
make catcher
./test/catcher_test.sh
```

---

### Lab 06: Potoki
Na tym laboratorium ćwiczyliśmy komunikację między procesami za pomocą potoków nienazwanych i nazwanych. Zaimplementowaliśmy dwa zadania: równoległe obliczanie całki metodą prostokątów z użyciem wielu procesów oraz komunikację dwóch programów przez potoki nazwane.

#### Programy:
- `pipe_1.c` – Oblicza całkę z funkcji 4/(x^2+1) w przedziale [0,1] metodą prostokątów, rozdzielając obliczenia na wiele procesów i zbierając wyniki przez potoki nienazwane.
- `pipe_2_IO.c` i `pipe_2_calculator.c` – Komunikacja przez potoki nazwane: pierwszy program przesyła dane do drugiego, drugi liczy całkę i odsyła wynik.

#### Budowanie i uruchamianie:
```bash
make pipe_1
./pipe_1 <szerokość_prostokąta> <max_procesów>
```
```bash
make pipe_2_IO
make pipe_2_calculator
./pipe_2_IO <start> <end> <width>
```
lub
```bash
make pipe_1
./test/pipe_1_test.sh
make pipe_2_IO
./test/pipe_2_test.sh
```

---

### Lab 07: Kolejka komunikatów
Na tym laboratorium poznaliśmy komunikację międzyprocesową za pomocą kolejek komunikatów System V. Zaimplementowaliśmy prosty system klient-serwer, w którym klienci mogą wysyłać wiadomości do serwera, a serwer przekazuje je do innych klientów.

#### Programy:
- `chat_server.c` – Serwer obsługujący wielu klientów, przydzielający im identyfikatory i przekazujący wiadomości.
- `chat_client.c` – Klient, który rejestruje się u serwera, wysyła i odbiera wiadomości.

#### Budowanie i uruchamianie:
```bash
make chat_server
make chat_client
./chat_server
./chat_client <server_key>
```
lub
```bash
make chat_server
./test/chat_test.sh
```

---

### Lab 08: Pamięć wspólna, Semafory
Na tym laboratorium ćwiczyliśmy synchronizację procesów za pomocą pamięci wspólnej i semaforów System V. Zaimplementowaliśmy system wydruku z wieloma użytkownikami i drukarkami, gdzie zadania są kolejkowane i synchronizowane.

#### Programy:
- `print_system.c` – Symuluje system wydruku z N użytkownikami i M drukarkami, korzystając z pamięci wspólnej i semaforów do synchronizacji dostępu do kolejki zadań.

#### Budowanie i uruchamianie:
```bash
make print_system
./print_system
```
lub
```bash
make print_system
./test/print_system_test.sh
```

---

### Lab 09: Wątki
Na tym laboratorium poznaliśmy podstawy programowania współbieżnego z użyciem wątków POSIX (pthread). Zaimplementowaliśmy równoległe obliczanie całki metodą prostokątów, rozdzielając pracę na wiele wątków.

#### Programy:
- `integral.c` – Oblicza całkę z funkcji 4/(x^2+1) w przedziale [0,1] metodą prostokątów, rozdzielając obliczenia na k wątków. Każdy wątek oblicza część całki, a następnie wyniki są sumowane.

#### Budowanie i uruchamianie:
```bash
make integral
./integral <szerokość_prostokąta> <liczba_wątków>
```
lub
```bash
make integral
./test/integral_test.sh
```

#### Przykładowe użycie:
```bash
./integral 0.0001 4
```
Powyższa komenda uruchamia program z szerokością prostokąta 0.0001 i 4 wątkami.

Program wypisuje wynik obliczonej całki oraz czas wykonania dla różnych liczby wątków, co pozwala zaobserwować wpływ współbieżności na wydajność obliczeń.

### Lab 10: Metody synchronizacji wątków (Problem lekarza)
Na tym laboratorium poznaliśmy zaawansowane metody synchronizacji wątków z użyciem mutexów i zmiennych warunkowych POSIX. Zaimplementowaliśmy symulację pracy szpitala, w którym lekarz obsługuje pacjentów, a farmaceuci dostarczają leki do apteczki. Program demonstruje synchronizację i komunikację między wieloma wątkami.

#### Programy:
- `doctor.c` – Symuluje pracę lekarza, pacjentów i farmaceutów w szpitalu. Pacjenci przychodzą na konsultacje, farmaceuci uzupełniają leki, a lekarz konsultuje pacjentów tylko, gdy zbierze się ich trójka i są dostępne leki.

#### Budowanie i uruchamianie:
```bash
make doctor
./doctor <liczba_pacjentów> <liczba_farmaceutów>
```
lub
```bash
make doctor
./test/doctor_test.sh
```

#### Przykładowe użycie:
```bash
./doctor 20 3
```
Powyższa komenda uruchamia symulację z 20 pacjentami i 3 farmaceutami.

Program wypisuje komunikaty o stanie szpitala, konsultacjach, dostawach leków i synchronizacji pracy wszystkich wątków.

---

### Lab 11: Sockety – część 1 (Prosty chat)
Na tym laboratorium poznaliśmy podstawy komunikacji sieciowej z użyciem socketów TCP/IP. Zaimplementowaliśmy prosty system czatu klient-serwer, w którym klienci mogą komunikować się ze sobą za pośrednictwem serwera.

#### Programy:
- `socket_chat_server.c` – Serwer czatu obsługujący wielu klientów, przekazujący wiadomości, listę aktywnych użytkowników oraz zarządzający połączeniami.
- `socket_chat_client.c` – Klient czatu, który łączy się z serwerem, wysyła i odbiera wiadomości, obsługuje polecenia (LIST, 2ALL, 2ONE, STOP).

#### Budowanie i uruchamianie:
```bash
make socket_chat_server
make socket_chat_client
./socket_chat_server <IP> <PORT>
./socket_chat_client <ID> <IP> <PORT>
```
lub
```bash
make socket_chat_server
./test/socket_chat_test.sh
```

#### Przykładowe użycie:
```bash
./socket_chat_server 127.0.0.1 12345
./socket_chat_client Anna 127.0.0.1 12345
./socket_chat_client Bob 127.0.0.1 12345
```
Klient po połączeniu może wpisywać polecenia:
- `LIST` – wyświetla listę aktywnych użytkowników,
- `2ALL <wiadomość>` – wysyła wiadomość do wszystkich,
- `2ONE <ID> <wiadomość>` – wysyła wiadomość do wybranego użytkownika,
- `STOP` – kończy pracę klienta.

Serwer cyklicznie sprawdza, czy klienci odpowiadają (pingowanie), a nieaktywni są usuwani z listy.

### Lab 12: Sockety - część 2
Na tym laboratorium zmodyfikowaliśmy kod chatu z poprzednich zajęć tak, aby korzystać z socketów i protokołu UDP.

#### Programy:
- `socket_chat_server_2.c`
- `socket_chat_client_2.c`

#### Budowanie i uruchamianie:
```bash
make socket_chat_server_2
make socket_chat_client_2
./socket_chat_server_2 <IP> <PORT>
./socket_chat_client_2 <ID> <IP> <PORT>
```
lub
```bash
make socket_chat_server_2
./test/socket_chat_2_test.sh
```

#### Przykładowe użycie:
```bash
./socket_chat_server_2 127.0.0.1 12345
./socket_chat_client_2 Anna 127.0.0.1 12345
./so
```
### Różnice między wersjami
>Jako, że funkcjonalność programu jest identyczna do programu z Labu 11, pozwolę sobie omówić różnice między programami.

1. **Protokół komunikacji**
   - **TCP** (`socket_chat_server.c`, `socket_chat_client.c`): Używa połączeniowego protokołu TCP (`SOCK_STREAM`). Każdy klient łączy się z serwerem przez `connect()`, a serwer akceptuje połączenia przez `accept()`. Komunikacja odbywa się przez deskryptory połączeń.
   - **UDP** (`socket_chat_server_2.c`, `socket_chat_client_2.c`): Używa bezpołączeniowego protokołu UDP (`SOCK_DGRAM`). Nie ma pojęcia połączenia – każda wiadomość jest wysyłana i odbierana osobno, z podaniem adresu odbiorcy/nadawcy.

2. **Obsługa klientów**
   - **TCP:** Serwer dla każdego klienta tworzy osobny wątek i przechowuje deskryptor socketu klienta. Komunikacja z klientem odbywa się przez ten deskryptor.
   - **UDP:** Serwer nie tworzy osobnych wątków dla klientów. Każda wiadomość od klienta jest odbierana w głównej pętli serwera, a identyfikacja klienta odbywa się na podstawie adresu (`struct sockaddr_in`). Serwer przechowuje adresy klientów w tablicy.

3. **Wysyłanie i odbieranie wiadomości**
   - **TCP:** Do wysyłania i odbierania używane są funkcje `send()` i `recv()` na połączonych socketach.
   - **UDP:** Do wysyłania i odbierania używane są funkcje `sendto()` i `recvfrom()`, gdzie zawsze podaje się adres odbiorcy/nadawcy.

4. **Rejestracja i identyfikacja klienta**
   - **TCP:** Klient po połączeniu wysyła swój identyfikator, który jest przypisywany do deskryptora socketu.
   - **UDP:** Klient wysyła swój identyfikator jako pierwszą wiadomość, a serwer przypisuje ten identyfikator do adresu klienta (`sockaddr_in`).

5. **Zarządzanie połączeniami**
   - **TCP:** Połączenie jest utrzymywane przez cały czas działania klienta. Rozłączenie klienta wykrywane jest przez zamknięcie socketu lub błąd w `recv()`.
   - **UDP:** Nie ma pojęcia połączenia – klient jest uznawany za aktywnego, jeśli regularnie wysyła wiadomości lub odpowiada na ping. Brak odpowiedzi skutkuje usunięciem klienta z listy.

6. **Pingowanie klientów**
   - **TCP:** Pingowanie polega na wysyłaniu wiadomości przez socket klienta. Brak odpowiedzi lub błąd w komunikacji skutkuje zamknięciem połączenia.
   - **UDP:** Pingowanie polega na wysyłaniu wiadomości na adres klienta. Brak odpowiedzi skutkuje usunięciem klienta z listy.

7. **Wątki**
   - **TCP:** Każdy klient ma swój własny wątek obsługi.
   - **UDP:** Cała obsługa klientów odbywa się w jednej głównej pętli serwera, bez dedykowanych wątków dla każdego klienta.

8. **Podsumowanie**
   - Wersje `_2` korzystają z UDP, co oznacza:
     - Brak połączeń – każda wiadomość jest niezależna.
     - Identyfikacja klienta po adresie IP i porcie.
     - Wysyłanie i odbieranie zawsze z podaniem adresu.
     - Prostsza obsługa wielu klientów, ale brak gwarancji dostarczenia i kolejności wiadomości.
     - Brak dedykowanych wątków dla każdego klienta po stronie serwera.