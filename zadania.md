# Zadania Systemy Operacyjne 2025
> Sporządził: Gabriel Kania
## Lab 01: Środowisko pracy
### Wprowadzenie

W tym ćwiczeniu zgłębimy kluczowe narzędzia i praktyki niezbędne do efektywnego tworzenia, budowania i debugowania aplikacji w języku C.

Pliki Makefile są nieodłącznym elementem procesu kompilacji w środowiskach Unix-like. Pozwalają one na zdefiniowanie zestawu instrukcji, które są potrzebne do budowania aplikacji, włączając w to kompilację źródeł, linkowanie bibliotek oraz zarządzanie zależnościami między plikami. Podczas tego ćwiczenia dowiemy się, jak tworzyć i zarządzać plikami Makefile, co pozwoli nam na zautomatyzowanie procesu kompilacji i ułatwi pracę nad projektami programistycznymi.

Środowiska programistyczne, zwłaszcza IDE (Integrated Development Environment), są niezastąpionymi narzędziami dla każdego programisty. W ćwiczeniu przyjrzymy się popularnym IDE dla języka C, jak również poznajemy ich funkcje i możliwości, które pomogą nam w efektywnym tworzeniu, debugowaniu i testowaniu aplikacji.

Techniki debugowania są nieodzowne w procesie rozwoju oprogramowania, zwłaszcza gdy napotykamy na błędy i trudności w naszym kodzie. W trakcie tego ćwiczenia zgłębimy różne metody debugowania w języku C, w tym korzystanie z debuggera, wstawianie punktów kontrolnych oraz analizowanie zmiennych działającego programu.

### Zadanie
1. Zapoznaj się z koncepcją plików Makefile: https://www.gnu.org/software/make/manual/html_node/Introduction.html
2. Zainstaluj/skonfiguruj IDE, w którym będziesz pracować przez resztę laboratoriów. (np. VS Code, Vim, etc.)
3. Stwórz nowy projekt w IDE.
4. Napisz prosty program countdown.c, który będzie w pętli odliczał od 10 do 0 i wypisywał aktualną liczbę na konsolę (każda liczba w nowej linii).
5. Stwórz plik Makefile, za pomocą którego skompilujesz swój program.
    Makefile powinien zawierać co najmniej trzy targety: all, countdown, clean.

    - all — powinien budować wszystkie targety (na razie tylko countdown, w kolejnych zadaniach będziemy dodawać nowe targety).
    - countdown — buduje program countdown.c
    - clean — usuwa wszystkie pliki binarne, czyści stan projektu

    Użyj zmiennych CC oraz CFLAGS do zdefiniowania kompilatora (gcc) i flag compilacji (-Wall, -std=c17, ...).

    Dodatkowo w Makefile powinien być specjalny target .PHONY.
6. Skompiluj i uruchom program.
7. Korzystając z gdb (lub lldb) zademonstruj poniższe:
    - zatrzymywanie programu (breakpoint) wewnątrz pętli
    - podejrzenie aktualnego indeks pętli
    - kontynuację wykonywania programu
    - wypisanie kolejnego indeksu
    - usunięnie breakpoint-a
    - kontynuowanie działania programu do końca

    https://sourceware.org/gdb/current/onlinedocs/gdb.html/
    https://sourceware.org/gdb/current/onlinedocs/gdb.html/Sample-Session.html#Sample-Session
    https://lldb.llvm.org/use/tutorial.html

8. Skonfiguruj swoje IDE do współpracy z Makefile.  Postaw breakpoint (graficznie, klikając) w środku pętli. Uruchom program w trybie Debug i zademonstruj wszystkie podpunkty z pkt. 7.

## Lab 02: Biblioteki w systemie Unix
### Wprowadzenie

Biblioteki są kluczowymi składnikami każdego systemu operacyjnego, a ich zrozumienie jest niezbędne dla skutecznego tworzenia i zarządzania aplikacjami. W ramach tego ćwiczenia przyjrzymy się trzem głównym typom bibliotek: statycznym, współdzielonym i dynamicznym.

Biblioteki statyczne są skompilowanymi plikami binarnymi, które są dołączane do programów podczas kompilacji. Są one integralną częścią aplikacji i wszystkie funkcje zawarte w bibliotece statycznej są kopiowane do pliku wykonywalnego programu. W przeciwieństwie do tego biblioteki współdzielone są ładowane do pamięci podczas uruchamiania programu i mogą być współużytkowane przez wiele aplikacji. Natomiast biblioteki dynamiczne są podobne do bibliotek współdzielonych, ale są ładowane do pamięci podczas uruchamiania programu i mogą być ładowane i usuwane dynamicznie w trakcie działania aplikacji.

Aby zademonstrować tworzenie i korzystanie z bibliotek zaimplementujemy bibliotekę pomagającą w badaniu problemu Collatza.

### Problem Collatza 
Problem Collatza (Collatz Conjecture), znana również jako problem 3n+1, to jedno z najbardziej znanych nierozwiązanych problemów w matematyce. Zakłada, że ​​startując od dowolnej dodatniej liczby całkowitej, można ją zredukować do 1, wykonując iteracyjne kroki na podstawie następujących reguł: jeśli liczba jest parzysta, podziel ją przez 2; jeśli jest nieparzysta, pomnóż przez 3 i dodaj 1. Mimo prostoty zasady, problem pozostaje nierozwiązany dla dowolnej liczby startowej, choć jest uznawany za prawdziwy ze względu na obserwacje empiryczne dla wielu początkowych wartości. 

### Zadanie
1. Stwórz bibliotekę w języku C wystawiającą klientom następujące dwie funkcje:
    1. `int collatz_conjecture(int input)` - funkcja realizująca regułę Collatza postaci:

        ![Collatz Conjecture](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAARgAAABTCAYAAABEZhiXAAAKrmlDQ1BJQ0MgUHJvZmlsZQAASImVlwdUk9kSgO///+kktIQISAm9CdIJICX0UKRXUQlJgFBiDAQVGyKLK7gWRKRYgVWKgqtSZK1YsC2CCtgXZFFQ18WCqKi8HzgEd9957503OZP5Mpk7M/c/9+ZMAKDQOCJRKiwPQJowQxzi7caIio5h4IcABBBAAsrAjsNNF7GCgvwBKjP27/KhB41G5Y7pZK5///6/igKPn84FAApCOZ6Xzk1D+SSqr7kicQYASBXq11mRIZrkqyjTxGiDKD+a5MRpHpnk+CnGYKZiwkLcUVYGgEDmcMSJAJB1UT8jk5uI5iF7oGwu5AmEKKOfgXNa2jIeymhdYIjGiFCezM+M/y5P4t9yxktzcjiJUp7ey5QQPATpolTOqv/zcfxvSUuVzNTQR5WcJPYJQS0dfWb3Upb5SVkYvzBwhgW8qfgpTpL4hM8wN909ZoZ5HA8/6drUhf4znCDwYkvzZLDDZpif7hk6w+JlIdJaCWJ31gxzxLN1JSnhUn8Sny3Nn5UUFjnDmYKIhTOcnhLqNxvjLvWLJSHS/vlCb7fZul7Svaelf7dfAVu6NiMpzEe6d85s/3whazZnepS0Nx7fw3M2JlwaL8pwk9YSpQZJ4/mp3lJ/emaodG0GeiBn1wZJn2EyxzdohoEH8AT+6IsBwoElsAYW6HsAABn8lZNnFLgvE60SCxKTMhgs9JbxGWwh12wew9Lc0hqAyTs7fSTe3Zu6ixCdMOsTofnt0TOPVM764lUBaEbPkQpx1qd7GAC5KACasrkScea0b/I6ASz6SyAHaEAFaAAdYAhM0c5sgSNwRTv2BYEgDESDJYALkkAaEIMVYA3YAPJAAdgOdoEysB9UgmpwFBwHzeA0uACugBugE3SDh6APDIKXYAR8AOMQBOEhCkSFVCBNSA8ygSwhJuQMeUL+UAgUDcVBiZAQkkBroI1QAVQIlUEHoRroF+gUdAG6BnVB96F+aBh6C32GEZgM02B1WB+eDzNhFuwHh8GL4UR4OZwF58Jb4RK4Aj4CN8EX4BtwN9wHv4RHEYDIIHRECzFFmIg7EojEIAmIGFmH5CPFSAVSj7Qi7cgdpA95hXzC4DBUDANjinHE+GDCMVzMcsw6zBZMGaYa04S5hLmD6ceMYL5hKVg1rAnWAcvGRmETsSuwedhi7CFsI/Yyths7iP2Aw+HoOAOcHc4HF41Lxq3GbcHtxTXgzuO6cAO4UTwer4I3wTvhA/EcfAY+D1+KP4I/h7+NH8R/JMgQNAmWBC9CDEFIyCEUE2oJZwm3Cc8J40R5oh7RgRhI5BFXEbcRq4itxFvEQeI4SYFkQHIihZGSSRtIJaR60mXSI9I7GRkZbRl7mWAZgUy2TInMMZmrMv0yn8iKZGOyOzmWLCFvJR8mnyffJ7+jUCj6FFdKDCWDspVSQ7lIeUL5KEuVNZNly/Jk18uWyzbJ3pZ9LUeU05NjyS2Ry5Irljshd0vulTxRXl/eXZ4jv06+XP6UfK/8qAJVwUIhUCFNYYtCrcI1hSFFvKK+oqciTzFXsVLxouIAFaHqUN2pXOpGahX1MnWQhqMZ0Ni0ZFoB7SitgzaipKhkrRShtFKpXOmMUh8doevT2fRU+jb6cXoP/fMc9TmsOfw5m+fUz7k9Z0x5rrKrMl85X7lBuVv5swpDxVMlRWWHSrPKY1WMqrFqsOoK1X2ql1VfzaXNdZzLnZs/9/jcB2qwmrFaiNpqtUq1m2qj6hrq3uoi9VL1i+qvNOgarhrJGkUaZzWGNamazpoCzSLNc5ovGEoMFiOVUcK4xBjRUtPy0ZJoHdTq0BrXNtAO187RbtB+rEPSYeok6BTptOmM6GrqBuiu0a3TfaBH1GPqJent1mvXG9M30I/U36TfrD9koGzANsgyqDN4ZEgxdDFcblhheNcIZ8Q0SjHaa9RpDBvbGCcZlxvfMoFNbE0EJntNuuZh59nPE86rmNdrSjZlmWaa1pn2m9HN/M1yzJrNXs/XnR8zf8f89vnfzG3MU82rzB9aKFr4WuRYtFq8tTS25FqWW961olh5Wa23arF6Y21izbfeZ33PhmoTYLPJps3mq62drdi23nbYTtcuzm6PXS+TxgxibmFetcfau9mvtz9t/8nB1iHD4bjDX46mjimOtY5DCwwW8BdULRhw0nbiOB106nNmOMc5H3Duc9Fy4bhUuDx11XHluR5yfc4yYiWzjrBeu5m7id0a3cbcHdzXup/3QDy8PfI9OjwVPcM9yzyfeGl7JXrVeY1423iv9j7vg/Xx89nh08tWZ3PZNewRXzvftb6X/Mh+oX5lfk/9jf3F/q0BcIBvwM6ARwv1FgoXNgeCQHbgzsDHQQZBy4N+DcYFBwWXBz8LsQhZE9IeSg1dGlob+iHMLWxb2MNww3BJeFuEXERsRE3EWKRHZGFkX9T8qLVRN6JVowXRLTH4mIiYQzGjizwX7Vo0GGsTmxfbs9hg8crF15aoLkldcmap3FLO0hNx2LjIuNq4L5xATgVnNJ4dvyd+hOvO3c19yXPlFfGG+U78Qv7zBKeEwoShRKfEnYnDSS5JxUmvBO6CMsGbZJ/k/cljKYEph1MmUiNTG9IIaXFpp4SKwhThpWUay1Yu6xKZiPJEfcsdlu9aPiL2Ex9Kh9IXp7dk0NDh6KbEUPKDpD/TObM88+OKiBUnViqsFK68ucp41eZVz7O8sn5ejVnNXd22RmvNhjX9a1lrD66D1sWva1uvsz53/WC2d3b1BtKGlA2/5ZjnFOa83xi5sTVXPTc7d+AH7x/q8mTzxHm9mxw37f8R86Pgx47NVptLN3/L5+VfLzAvKC74soW75fpPFj+V/DSxNWFrxzbbbfu247YLt/fscNlRXahQmFU4sDNgZ1MRoyi/6P2upbuuFVsX799N2i3Z3VfiX9JSqlu6vfRLWVJZd7lbecMetT2b94zt5e29vc91X/1+9f0F+z8fEBy4d9D7YFOFfkVxJa4ys/JZVURV+8/Mn2sOqR4qOPT1sPBwX3VI9aUau5qaWrXabXVwnaRu+Ejskc6jHkdb6k3rDzbQGwqOgWOSYy9+iful57jf8bYTzBP1J/VO7mmkNuY3QU2rmkaak5r7WqJbuk75nmprdWxt/NXs18OntU6Xn1E6s+0s6Wzu2YlzWedGz4vOv7qQeGGgbWnbw4tRF+9eCr7Ucdnv8tUrXlcutrPaz111unr6msO1U9eZ15tv2N5oumlzs/E3m98aO2w7mm7Z3WrptO9s7VrQdfa2y+0LdzzuXLnLvnuje2F3V094z73e2N6+e7x7Q/dT7795kPlg/GH2I+yj/Mfyj4ufqD2p+N3o94Y+274z/R79N5+GPn04wB14+Uf6H18Gc59RnhU/13xeM2Q5dHrYa7jzxaIXgy9FL8df5f2p8Oee14avT/7l+tfNkaiRwTfiNxNvt7xTeXf4vfX7ttGg0Scf0j6Mj+V/VPlY/Yn5qf1z5Ofn4yu+4L+UfDX62vrN79ujibSJCRFHzJkaBRBU4YQEAN6icwIlGgBqJwCkRdMz9ZRA0/8Dpgj8J56eu6fEFoDK8wCEZQPgj9pS1OqjKucKQBCqYa4AtrKS6sz8OzWrT4r8EQAOZNn4hIV27th5APxDpuf47/r+pwXSrH+z/wLwEgVVli37YwAAAFZlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA5KGAAcAAAASAAAARKACAAQAAAABAAABGKADAAQAAAABAAAAUwAAAABBU0NJSQAAAFNjcmVlbnNob3QbEwV8AAAB1WlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNi4wLjAiPgogICA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPgogICAgICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgICAgICAgICB4bWxuczpleGlmPSJodHRwOi8vbnMuYWRvYmUuY29tL2V4aWYvMS4wLyI+CiAgICAgICAgIDxleGlmOlBpeGVsWURpbWVuc2lvbj44MzwvZXhpZjpQaXhlbFlEaW1lbnNpb24+CiAgICAgICAgIDxleGlmOlBpeGVsWERpbWVuc2lvbj4yODA8L2V4aWY6UGl4ZWxYRGltZW5zaW9uPgogICAgICAgICA8ZXhpZjpVc2VyQ29tbWVudD5TY3JlZW5zaG90PC9leGlmOlVzZXJDb21tZW50PgogICAgICA8L3JkZjpEZXNjcmlwdGlvbj4KICAgPC9yZGY6UkRGPgo8L3g6eG1wbWV0YT4KaEa6IwAAGkNJREFUeAHtnQWY3DYThpUyc1NuU2ZmZkyZGVJIKWXGlClt2pRSZmampE2ZmZmZmat/Xv2ZjdZn76593tzmMvM8d2uQZPmzPB7SqIsXckaGgCFgCDQBgRGa0KY1aQgYAoZAQMAYjA0EQ8AQaBoCxmCaBq01bAgYAsZgbAwYAoZA0xAwBtM0aK1hQ8AQMAZjY8AQMASahoAxmKZBaw0bAoaAMRgbA4aAIdA0BIzBNA1aa9gQMASMwdgYMAQMgaYhYAymadBaw4aAIWAMxsaAIWAINA0BYzBNg9YaNgQMAWMwGWPAJplnAGOHDYEcCBiDSYD122+/uX333dfNNttsiTO2awgYAnkRGClvhc5c/rPPPnMrrbSSG2+88dxDDz3UmW/V7s0QGCoIdLGEU//H+eeff3aLLrqo+/vvv90TTzzhxh9//KHyAOwihkBnRsBUpMFP94QTTnCvvvqq69evX1OZyyuvvOKeeuqpzjymhot7u+uuu9xLL73UKe6V8Thw4MCm3IsxGIH1n3/+cWeeeaabbrrp3Morr9wUoLXRc845pw0D++GHH9xbb70V+qHlWvn3448/dp9//nmbLt52221u0003dcsuu6x75JFH2pxvlQO//vqre/PNN92///5bqEu8jFdccYWba665CtUvWunZZ591++23n+vZs2ch5vbdd9+5d955p819zzfffO7UU091zz//fNGuZddDRRre6dFHHyUvsd9ss82aCsVff/3lu3fvXrkG+zvvvLNfc801/ZJLLunF9uNPP/30yvlW3JCX0k844YR+mmmmqerejTfeGI59/fXXfowxxvCbb7551flW2JEPid9tt938uuuu6w888EC/4IIL+scffzxX13766Sc/yyyzeLHX5apXRuH33nvPH3rooWGs3nvvvQ03+fvvv/sePXr4tdde2y+yyCJ+ookm8hdeeGFV/Q8//NDPPPPMXphv1fH27rj2NtAZ6l9//fXhoe25555NvZ1bb73V9+3bt3KN448/3vOnpIPnuuuu00Mt+bvNNtv4XXbZpapvyy+/fOXYww8/7BmwrUZgv/TSS1e6dccdd/gpp5zSwzQapaOOOsrvtddejRYvvdynn36am8HATPv371/py+677+67dOni77nnnsoxNrbccksvknzVsfbuGIMRBK+99trw0Jo9cJCQGCBKSy21lB955JErL6OoSuHBx1KOlm313wUWWCBIBa3cT1Fp/NFHH13pIhLkiCOO6K+55prKsVobSEBTTDGFFydArWJNPffJJ5+EsZpkDrUuyn2PPvro/ttvvw3FPvjgg9CGqLNV1ZCKZp111qpj7d0xNzW60eCVW4SrZ+uS0RnsJRj4Vl99dTfaaKM5dGN028UWW8xNNdVUUckhmz/++KP75Zdf3OSTT145uOKKKzphMG6CCSYIx8Ydd1w35phjOtzlrUrYLmSAOhG7nYjcoZvo7sQPffPNN0GPF/Wpck/xfTz55JMOO8Aqq6wS7E2inrivvvrKrbDCCiE0IC4bb19wwQVOpL/4UOY29onVVlutzfkvv/wyPLOddtqpcg7s8RYSkrDhhhtWjmdtvPjii06kHTf//PNXivBMn3nmGcfzJXaKEIcBAwaEcbHqqquGX4yoH330kRPpyU088cSVumzgtaT822+/7WaffXa33HLLueQ4xOZFG9Sdfvrpq+o3skM/RFJzY401Vig+6aSThmskx9lCCy0U+sH1ssZxI9eLyxiDETSUwcTAZG0zUPbZZ5/wgC677LLw4MQm4WAOc8wxh2MQduvWrU11sVG4tdZaq+r4IYcc4vhTgmkxYDGStirxMoiq4XDrK4M577zznNheHAyEl2PrrbcOLv/4Hh544IFgGKXMnXfeGV7IueeeOzCrgw46yL388stu1FFHjatUtrfddlvHX3sIBgPBVGIaZZRR3BdffBEfytx+4YUXwgs+0khDXhuM3RhIb7nlFrfVVluFDwQMs0+fPu7ss8928847b2AMXbt2DXVFLXNibwvXoC5GcXDk2A033BCwveqqq9zYY48dypxyyikOBotxFwZ+4oknZvYv64So4VWneAaM+eQ4YwzPMMMM7rHHHnMbbbRRVZ3CO+0VgTpD/auvvjqIjHvvvXfd2+ndu7eXL55HjxXpxT/44IOhDsYx9Nos+wmGXFSgWiRfUS9fKC8DqVaxDj8ng9zLC1PVD/mq+z322KPqWLyz8cYbe5FWvAxqP9lkk/n3338/nJbBHLAXySguXvq2MMZwnYsuuqiqbflSe+xHjRA2siz1dZxxxvGovEqXX355uJ4wXz3kxUPpN9lkk8o+Rtcddtihss+GSF8VHO+77z4/wggj+Ndff71SRphDaPfuu++uHMu7IRKkF2kp1fa0xBJL+JNPPjlvk5nlh7Diwixq+KpIMN7iiy8epJgtttjCyaAKABBDIyi7qaeeug0gYncJXza+EFl08cUXB/VC9GCHRNTKJHaL1O5x/1mEVMP9P/300+64446rSHnghhSBCJ9FuIUbdaHyVY5VGG1TbBBhM+maJkRBvF5arOYvahAqbBoh1TAulFCdoeQxpBAI9eTmm28OUk44MPgfatK5557r5CV3l1xyiRObjxOvVaUIKlh7CGmLazPOVEqK26N9sdXEh9q1bQwmJ3xMJWCgMeB79epVqU3glXyZUwe3SEg1dXxsAOeff74T74ubZJJJgp4vX8RK251h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XPhIsuJoMg82H4Ayly42CUaeSNZgSHtYeISIkI2jZBkUEKoSRJQlXyDd5xi5cJB0GBjJJTs53wzKOwjb0weuVe96fEGZHItHAxo0aFCQaJgmgiqZJZrDiDjXyF+er2+9/ibxwPuSlS4Ez2Hcnm7rL23F2zAPlrnFoBoTTFjsNAEjjMJcL2YyeKQgcZnH1WpuUx/DOssDoRqxxC2R86hfSUJ1E/tM5bB+DJE+i5BJMIKaMph4AGSBidrDg8cljW0AbwhLavbq1SvM2aIe0yZY+jV2Z6Iq4aKkHGsiN4tOO+20IFrjKoUJSvBb6CtSQhmEd4TcyeTvQUJjfSA8ZYjzMF88SqS9SFtYnT6xjrUSePC1hvGAY9bysSxpOmDAAK1W8xfPHbmg04iF5vH20Q+IRO9IEmQ3VIkirZ4eQ3LlYwLDU3cz+9w/WJO3GjUJO8lZZ50VqnG/eJdYhI9xwRjDRkfCM5gqqiNeR2wvuOMZM5onSYyxwTsFA2ZRQ5iLShzYaOhHI9ItXjjcz4yFmPr06RPvBgbOc6QvSoxxmB6Se3xcz9f7zZxNXa9iZzrPwOBhMtDqzbvivnHRMkeLrw+MA8MtBj4lXJfEg8S2BhaqQzxlANUzQmo7/NIvXrBGiXgJxF/V9+krDFRjTxptJ6scKgKGbQhpgdgNxO94bWrif2AcSSJzIF/t+BwvPWI5X1eVbJL1ytqHifE1x84BJmDDPUjgZE0bUHx97Gv9+vWrPFtUHAzQtIlUgb0IO5rmjkaygXnhvgcjvSbGaO6X87joYVSMi7S8SzAHYmVwIaMqMl5Rc7D34QqvRzAJfWZxWbI+ooopyfSBsABbrAZyT1wftVY/xFq+oV/hqMM9yRcUd4vHbdhqxLwZo9ZBgOehUyJap1fl9ESyTXpJBldOY4NbMQlGOAtfEaIkMW4hIhoZAlkIIB2iKhHNHdsqssoPK8dJ7I/tB2kHaawsMiOvIAmgPXv2DGLqwIEDy8LW2umECOBWxgaFnSmPQbmVoWCKB7YzjL9lMhfu2SSYwU8eAyUBTejJGON0zkcrDwzrW8chQMg+ht40t3rH9arYlTHgYrNLmwdVrMUhtYzBDMEiuAQJhGOxOdZLSot0jIrbpiFgCNRBwFSkCCDiHNBB8YLEMSxREds0BAyBHAiYBJMBFvq1ul4zithhQ8AQqIOAMZg6ANlpQ8AQKI6AqUjFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAcBYzB1ALLThoAhUBwBYzDFsbOahoAhUAeB/wEb10m91XQ8OwAAAABJRU5ErkJggg==)
        
        Funkcja ta przyjmuje jedną liczbę typu całkowitego. Jeżeli jest ona parzysta, podziel ją przez 2 i zwróć wynik. Jeżeli jest nieparzysta, pomnóż ją przez 3 i dodaj 1, a następnie zwróć wynik.

    2. `int test_collatz_convergence(int input, int max_iter, int *steps)` - funkcja sprawdzająca po ilu wywołaniach collatz_conjecture zbiega się do 1. Powinna ona wywoływać regułę Collatza najpierw na liczbie wejściowej a później na wyniku otrzymanym z reguły. W celu ochrony przed zbyt długim zapętlaniem się funkcji drugi parametr stanowi górną granicę liczby iteracji. Steps jest wyściowym argumentem zawieracjącym listę wynikową. Funkcja zwraca rozmiar tablicy steps czyli liczbę wykonanych kroków.
    3. Zwróć do wywołującej funkcji sekwencję liczb prowadzącą do redukcji `input` do 1. W przypadku, gdy nie było to możliwe w `max_iter`, zwróć z funkcji 0.
2. W pliku **makefile** utwórz dwa wpisy: do kompilacji statycznej biblioteki i do kompilacji współdzielonej.
3. Napisz klienta korzystającego z kodu biblioteki, klient powinien sprawdzać kilka liczb, wykorzystując `test_collatz_convergence`, tj. po ilu iteracjach wynik zbiegnie się do 1 i wypisać całą sekwencję redukcji `input` do 1, gdy dało się to osiągnąć w `max_iter`. W przeciwnym wypadku wypisz komunikat o niepowodzeniu. Klient powinien korzystać z biblioteki na 3 sposoby:
    1. Jako biblioteka statyczna
    2. Jako biblioteka współdzielona (linkowana dynamicznie)
    3. Jako biblioteka ładowana dynamicznie

    Dla każdego z wariantów utwórz odpowiedni wpis w **Makefile**. Do realizacji biblioteki dynamicznej użyj definicji stałej (-D) i dyrektywy preprocesora, aby zmodyfikować sposób działania klienta.

## Lab 03: Operacje na plikach
### Zadanie: Pliki lustrzane
Napisz program o nazwie `flipper`, który na wejściu przyjmie ścieżkę do katalogu źródłowego oraz katalogu wynikowego. Program będzie przeszukiwał katalog źródłowy w poszukiwaniu plików tekstowych, otwierał je i odwracał kolejność znaków w każdej linii. Następnie zmodyfikowane pliki zostaną zapisane w katalogu wynikowym. Jako dane wejściowe wykorzystaj katalog z archiwum **art.tgz** dołączonego do tego ćwiczenia (folder `resources`).

Dodatkowo:
1. Przed odwróceniem wyznacz sumę kontrolna pliku wejściowego (prosta suma wartości bajtów).
2. Po odwróceniu i zapisaniu do nowego pliku ponownie wyznacz sumę kontrolną.
    - Jeśli suma pozostaje taka sama, oznacza to że dane nie zostały utracone, mimo że plik jest odwrócony.
3. Następnie należy użyć polecenia `md5sum` do sprawdzenia integralności danych. 
    - Po dwukrotnym odwróceniu skrót MD5 powinien być identyczny.

## Lab 04: Tworzenie procesów. Środowisko procesu, sterowanie procesami
### Zadanie 1
Napisz program, który przyjmuje jeden argument: *argv[1]*. Program ma utworzyć *argv[1]* procesów potomnych. Każdy proces potomny ma wypisać na standardowym wyjściu w jednym wierszu dwa identyfikatory: **identyfikator procesu macierzystego** i **swój własny**. Na końcu standardowego wyjścia proces macierzysty ma wypisać *argv[1]*. 

Wskazówka: aby program na pewno wypisywał *argv[1]* jako ostatni wiersz standardowego wyjścia, należy użyć funkcji systemowej `wait()`.
### Zadanie 2
Napisz program, który przyjmuje jeden argument: *argv[1]* — ścieżkę katalogu. Program powinien wypisać na standardowym wyjściu swoją nazwę, korzystając z funkcji `printf()`. Zadeklaruj zmienną globalną **global**, a następnie zmienną lokalną **local**. W zależności od zwróconej wartości przez `fork()` dokonaj obsługi błędu, wykonaj proces rodzica / proces potomny. 

W procesie potomnym:
1. wyświetl komunikat `child process`,
2. dokonaj inkrementacji zmiennych **global** i **local**,
3. wyświetl komunikat `child pid = %d, parent pid = %d`
4. wyświetl komunikat `child's local = %d, child's global = %d`
5. wykonaj program `/bin/ls` z argumentem *argv[1]*, korzystając z funkcji `execl()`, zwracając przy tym jej **kod błędu**.

W procesie rodzica:
1. wyświetl komunikat `parent process`
2. wyświetl komunikat `parent pid = %d, child pid = %d`
3. wyświetl komunikat `Child exit code: %d`
4. wyświetl komunikat `Parent's local = %d, parent's global = %d`
5. zwróć stosowny **kod błędu**.

## Lab 05: Sygnały
### Przydatne informacje
Rodzaje sygnałów: `SIGINT, SIGQUIT, SIGKILL, SIGTSTP, SIGSTOP, SIGTERM, SIGSEGV, SIGHUP, SIGALARM, SIGCHLD, SIGUSR1, SIGUSR2`

Sygnały czasu rzeczywistego: `SIGRTMIN, SIGRTMIN+n, SIGRTMAX`

Przydatne polecenia Unix: `kill, ps`

Przydatne funkcje systemowe: `kill, raise, sigqueue, signal, sigaction, sigemptyset, sigfillset, sigaddset, sigdelset, sigismember, sigprocmask, sigpending, pause, sigsuspend`

### Zadanie 1
Napisz program demonstrujący różne reakcje na przykładowy sygnał **SIGUSR1** w zależności od ustawionych dyspozycji. Reakcja na sygnał **SIGUSR1** programu powinna zależeć od wartości argumentu z linii poleceń. Argument ten może przyjmować wartości: `none, ignore, handler, mask`. Program w zależności od parametru odpowiednio: 
- **nie** zmienia reakcji na sygnał
- ustawia **ignorowanie** 
- instaluje **handler** obsługujący sygnał (działający w ten sposób, że wypisuje komunikat o jego otrzymaniu)
- **maskuje** ten sygnał oraz sprawdza czy wiszący/oczekujący sygnał jest widoczny. 

Następnie przy pomocy funkcji `raise` wysyła sygnał do samego siebie oraz wykonuje odpowiednie dla danej opcji, opisane wyżej działania.

### Zadanie 2
Napisz dwa programy: `sender.c` program wysyłający sygnały SIGUSR1 i `catcher.c` - program odbierający te sygnały. Program `catcher` jest uruchamiany jako pierwszy, wypisuje swój numer **PID** i czeka na sygnały **SIGUSR1**. Po każdorazowym odebraniu sygnału **SIGUSR1** przez program `catcher` powinno nastąpić potwierdzenie odbioru tego sygnału. W tym celu, `catcher` wysyła do `sendera` sygnał **SIGUSR1** informujący o odbiorze sygnału. `Sender` powinien wysłać kolejny sygnał dopiero po uzyskaniu tego potwierdzenia. Czekanie na takie potwierdzenie może odbywać się wywołując funkcję `sigsuspend`. Wraz z każdym sygnałem przesłanym przez sender powinien zostać przesłany tryb pracy programu `catcher` za pomocą funkcji `sigqueue`. Możliwe tryby pracy:
- 1 - wypisanie na standardowym wyjściu liczby otrzymanych żądań zmiany trybu pracy od początku działania programu `catcher`
- 2 - wypisywanie na standardowym wyjściu kolejnej liczby co jedną sekundę aż do momentu otrzymania innego trybu pracy
- 3 - ustawienie ignorowania przez program `catcher` reakcji na **Ctrl+C**
- 4 - ustawienie reakcji na **Ctrl+C** przez program catcher jako wypisanie tekstu: *Wciśnięto CTRL+C*
- 5 – zakończenie działania programu `catcher`.

**PID sendera** catcher pobiera ze struktury `siginfo_t` po przechwyceniu od niego sygnału. Program `sender` jako pierwszy parametr przyjmuje **PID procesu catcher**. Drugi parametr określa tryb pracy procesu `catcher` - w jednym wywołaniu może być przekazany jeden taki tryb. Program `catcher` działa aż do momentu otrzymania trybu 3 (zakończenie działania programu `catcher`). Program `sender` działa do momentu otrzymania potwierdzenia otrzymania przez `catcher` przesłanego trybu, po czym kończy pracę. Program `sender` można wywołać wielokrotnie aż do zakończenia pracy przez `catcher`.

**UWAGA! W żaden sposób nie opóźniamy wysyłania sygnałów, wszelkie "gubienie" sygnałów jest zjawiskiem naturalnym.**
## Lab 06: Potoki
### Zadanie 1
Napisz program, który liczy numerycznie wartość całki oznaczonej z funkcji 4 / (x^2 + 1) w przedziale od 0 do 1 **metodą prostokątów** (z definicji całki oznaczonej Riemanna). Pierwszy parametr programu to **szerokość** każdego prostokąta, określająca dokładność obliczeń. Obliczenia należy rozdzielić na **k procesów potomnych**, tak by każdy z procesów liczył inny fragment ustalonego wyżej przedziału. Każdy z procesów powinien wynik swojej części obliczeń przesłać przez **potok nienazwany** do procesu **macierzystego**. Każdy proces potomny do komunikacji z procesem macierzystym powinien używać **osobnego potoku**. Proces macierzysty powinien oczekiwać na wyniki uzyskane od **wszystkich** procesów potomnych po czym powinien dodać te wyniki cząstkowe i wyświetlić wynik na standardowym wyjściu wraz z czasem wykonania oraz odpowiadającą mu wartością k. 

Program powinien przeprowadzić powyższe obliczenia dla wartości k=1,2,...,n (gdzie n to drugi parametr wywołania programu). W ten sposób w wyniku działania programu na standardowym wyjściu wypisane zostaną wyniki obliczeń oraz czasy wykonania tych obliczeń przy wykorzystaniu jednego procesu, dwóch procesów, trzech procesów oraz kolejnych liczb wykorzystanych procesów aż do n.

Dokładności obliczeń należy dobrać w ten sposób by obliczenia trwały co najmniej kilka sekund.

### Zadanie 2
Napisz program, który liczy numerycznie wartość całki oznaczonej tak jak w **zadaniu 1**. Obliczenia powinny zostać przeprowadzone w ten sposób, że pierwszy program czyta ze standardowego wejścia przedział w jakim całka ma być policzona a następnie przesyła przez **potok nazwany** do programu drugiego wartości określające ten przedział. Drugi program po otrzymaniu informacji liczy całkę w otrzymanym przedziale i odsyła do programu pierwszego wynik obliczeń. Po otrzymaniu wyniku obliczeń, program pierwszy wyświetla wynik.

## Lab 07: Kolejki komunikatów
### Zadanie

Napisz prosty program typu klient-serwer, w którym komunikacja zrealizowana jest za pomocą kolejek komunikatów.
Serwer po uruchomieniu tworzy nową kolejkę komunikatów. Za pomocą tej kolejki klienci będą wysyłać komunikaty do serwera. Komunikaty wysyłane przez klientów mogą zawierać polecenie oznaczające pierwsze nawiązanie połaczenia z serwerem (INIT) lub jeśli wcześniej połączenie zostało już nawiązane: identyfikator klienta wraz z komunikatem, który ma zostać przekazany przez serwer do wszystkich pozostałych klientów. W odpowiedzi na polecenie INIT, serwer ma przesłać identyfikator nadany nowemu klientowi.

Klient bezpośrednio po uruchomieniu tworzy kolejkę z unikalnym kluczem IPC i wysyła jej klucz do serwera wraz z komunikatem INIT. Po otrzymaniu takiego komunikatu, serwer otwiera kolejkę klienta, przydziela klientowi identyfikator (np. numer w kolejności zgłoszeń) i odsyła ten identyfikator do klienta (komunikacja w kierunku serwer->klient odbywa się za pomocą kolejki klienta). Po otrzymaniu identyfikatora, klient może wysłać do serwera komunikaty, które serwer będzie przesyłał do wszystkich pozostałych klientów. Komunikaty te są czytane ze standardowego wejścia. Klient po uruchomieniu tworzy drugi proces, który powinien odbierać komunikaty wysyłane przez serwer (przy użyciu kolejki komunikatów klienta) i wyświetlać te komunikaty na standardowym wyjściu.

Klient i serwer należy napisać w postaci osobnych programów. Serwer musi być w stanie pracować jednocześnie z wieloma klientami. Dla uproszczenia można przyjąć, że serwer przechowuje informacje o klientach w statycznej tablicy (rozmiar tablicy ogranicza liczbę klientów, którzy mogą się zgłosić do serwera).

Powyższe zadanie można zrealizować wykorzystując mechanizmy System V lub POSIX.

## Lab 08: Pamieć wspólna, Semafory
### Zadanie
Wykorzystując semafory i pamięć wspólną z IPC Systemu V lub standardu POSIX napisz program symulujący działanie systemu wydruku: 

System składa się z N użytkowników oraz M drukarek. Każdy z użytkowników może wysłać do systemu zadanie wydruku tekstu składającego się z 10 znaków. Drukarka, która nie jest aktualnie zajęta, podejmuje się zadania "wydruku" tekstu danego zadania. Wydruk w zadaniu polega na wypisaniu na standardowym wyjściu znaków wysłanych wcześniej do wydruku w ten sposób, że każdy następny znak wpisywany jest co jedną sekundę. Jeżeli wszystkie drukarki są zajęte to zlecenia wydruku są kolejkowane w opisywanym systemie wydruku. Jeżeli kolejka jest pełna to użytkownik chcący zlecić zadanie wydruku czeka do momentu gdy będzie można zlecenie wpisać do kolejki.

Każdy z N użytkowników powinien przesyłać do systemu wydruku zadanie wydruku 10 losowych znaków (od 'a' do 'z') a następnie odczekać losową liczbe sekund. Zadania zlecenia wydruku i odczekania powinny być wykonane w nieskończonej pętli. 
Należy zsynchronizować prace użytkowników oraz drukarek. Należy użyć mechanizmów System V lub POSIX.

## Lab 09: Wątki
### Zadanie
Napisz program, który liczy numerycznie wartość całki oznaczonej z funkcji 4/(x^2 + 1) w przedziale od 0 do 1 metodą prostokątów (z definicji całki oznaczonej Riemanna). Program będzie podobny do zadania 1 w zestawie 6, jednak do przyspieszenia obliczeń zamiast procesów użyjemy wątki. Pierwszy parametr programu to szerokość każdego prostokąta, określająca dokładność obliczeń. Obliczenia należy rozdzielić na k wątków, tak by każdy z nich liczył inny fragment ustalonego wyżej przedziału. Każdy z wątków powinien wynik swojej części obliczeń umieścić w odpowiednim miejscu przeznaczonej do tego tablicy wyników. Wątek główny powinien oczekiwać na wyniki uzyskane od wszystkich wątków. Ponieważ dopiero na następnym laboratorium zapoznamy się z metodami synchronizacji wątków, można do tego celu z pominięciem tych mechanizmów użyć tablicy, w której każdy z wątków liczących po umieszczeniu wyników swoich obliczeń w tablicy wyników umieszcza np wartość 1. Nazwijmy tę tablicę tablicą gotowości. Wątek główny sprawdza zatem czy wszystkie wartości w tablicy gotowości wynoszą 1 i jeśli tak to dodaje wyniki z tablicy wyników i wyświetla wynik na standardowym wyjściu. Zamiast powyższego można również przed dodaniem wyników zwróconych przez wątki czekać na zakończenie wszystkich wątków.

## Lab 10: Metody synchronizacji wątków
### Zadanie: Problem lekarza

W ramach zadania należy zaimplementować rozwiązanie problemu synchronizacji w środowisku szpitalnym.

W małym szpitalu dyżuruje jeden lekarz, który obsługuje dwie grupy:

- Pacjentów (np. 20 - ilosc pacjentów powinna byc konfigurowanlna), którzy przychodzą na konsultację,

- Farmaceutów(np. 3 - ilosc farmaceutów powinna byc konfigurowanlna), którzy dostarczają leki do apteczki w szpitalu.

Lekarz jest człowiekiem bardzo zapracowanym i przemęczonym, więc kiedy idzie spać to śpi tak długo jak tylko może. Lekarz jak wspomniano jest zapracowany, więc przeprowadza konsultację tylko jeśli zbierze się dokładnie 3 pacjentów. Po konsultacji lekarz wraca do spania. Z powodu prawnie narzuconych restrykcji po pandemi w szpitalu może przebywać maksymalnie 3 pacjentów, a reszta nie jest w ogóle wpuszczana do szpitala.

Apteczka (leki) ma ograniczoną pojemność (np. 6), a każda konsultacja zużywa 3 jednostki leku (po jednym dla każdego pacjenta na konsultacji). Uzupełnianie apteczki jest możliwe dzięki farmaceutom.
Lekarz może przyjąć pacjentów tylko, jeśli ma dostępną wystarczającą ilość leków w apteczce (conajmniej 3).


Komunikaty 

Lekarz, pacjenci oraz farmaceuci powinni informować w formie komunikatów (informacji na standardowe wyjście o aktualnych etapach). Zmienne do komunikatów: TIME - dokładny czas w systemie, PATIENT_ID - identyfikator pacjenta, PHARMACIST_ID - identyfikator farmaceuty

#### Zachowania pacjentów:

- Przychodzą do szpitala po losowym czasie (2–5s).

    >(Komunikat: [TIME] - Pacjent(PATIENT_ID): Ide do szpitala, bede za _ s).

- Jeśli w poczekalni czeka już 3 pacjentów – wracają później (robią sobie krótki spacer dookoła szpitala i znowu sprawdzają poczekalnie, jeśli dalej zajęta to znowu)

    >(Komunikat: [TIME] - Pacjent(PATIENT_ID): za dużo pacjentów, wracam później za _ s).

- W przeciwnym razie siadaja w poczekalni

    >(Komunikat: [TIME] - Pacjent(PATIENT_ID): czeka _ pacjentów na lekarza).

- Jeśli są trzecim pacjentem w poczekalni to budzą lekarza

    >(Komunikat: [TIME] - Pacjent(PATIENT_ID): budzę lekarza).

- Po konsultacji wracają do domu

    >(Komunikat: [TIME] - Pacjent(PATIENT_ID): kończę wizytę).


#### Zachowania farmaceutów:

- Przybywają z nową dostawą po losowym czasie (5–15s).

    >(Komunikat: [TIME] - Farmaceuta(PHARMACIST_ID): ide do szpitala, bede za _ s).

- Jeśli apteczka jest pełna – czekają na jej opróżnienie.

    >(Komunikat: [TIME] - Farmaceuta(PHARMACIST_ID): czekam na oproznienie apteczki).

- Jeśli apteczka jest pusta lub się kończy (ilość leków < 3):

    - Budzą lekarza

        >(Komunikat: [TIME] - Farmaceuta(PHARMACIST_ID): budzę lekarza).

    - Dostarczają nową partię leków

        >(Komunikat: [TIME] - Farmaceuta(PHARMACIST_ID): dostarczam leki).

- Po dostawie kończą pracę

    >(Komunikat: [TIME] - Farmaceuta(PHARMACIST_ID): zakończyłem dostawę).


#### Zachowania lekarza:

- Śpi, dopóki nie wystąpi jeden z warunków:

    - 3 pacjentów czeka, i leki >= 3

    - farmaceuta czeka, i apteczka sie kończy leki < 3

- Budzi się
    
    >(Komunikat: [TIME] - Lekarz: budzę się).

- Jeśli są pacjenci i leki:

    - Konsultuje ich

        >(Komunikat: [TIME] - Lekarz: konsultuję pacjentów PATIENT_ID1, PATIENT_ID2, PATIENT_ID3),

    - Zużywa 3 leki,

    - Konsultacja trwa 2–4s.

- W przeciwnym razie, jeśli są farmaceuci i miejsce w apteczce:

    - Odbiera dostawę

        >(Komunikat: [TIME] - Lekarz: przyjmuję dostawę leków),

    - Uzupełnia apteczke,

    - Dostawa trwa 1–3s.

- Po wykonaniu działania wraca spać

    > (Komunikat: [TIME] - Lekarz: zasypiam.


Program należy zaimplementować korzystając z wątków (pthread) i mechanizmów synchronizacji biblioteki POSIX Threads (mutex, condition variable). Po uruchomieniu programu wątek główny tworzy wątki dla Lekarza, Pacjentów oraz Farmaceutów. Ilość pacjentów i farmaceutów powinna być możliwa do przekazania jako parametr uruchomieniowy programu (pierwszy parametr to ilość pacjentów, drugi parametr to ilość farmaceutów). Praca lekarza się kończy, gdy nie ma więcej pacjentów do wyleczenia. Do spania Lekarza powinny być wykorzystane Warunki Sprawdzające (Condition Variables). 

Użycie odpowiednich mechanizmów ma zagwarantować niedopouszczenie, np. do zdarzeń: Lekarz śpi chociaż czeka na niego 3 pacjentów lub Farmaceuta nie jest w stanie uzupełnić leków, więcej niż jeden farmaceuta uzupełnia apteczke itp.

## Lab 11: Sockety - część 1
### Zadanie: Prosty chat

Napisz prosty chat typu klient-serwer w którym komunikacja pomiędzy uczestnikami czatu / klientami / klientami i serwerem realizowana jest za pośrednictwem socketów z użyciem protokołu strumieniowego.

Adres / port serwera podawany jest jako argument jego uruchomienia

Klient przyjmuje jako swoje argumenty:

- swoją nazwę/identyfikator (string o z góry ograniczonej długości)
- adres serwera (adres IPv4 i numer portu) 

Protokół komunikacyjny powinien obsługiwać następujące operacje:

- LIST: 

    Pobranie z serwera i wylistowanie wszystkich aktywnych klientów

- 2ALL string:

    Wysłania wiadomości do wszystkich pozostałych klientów. Klient wysyła ciąg znaków do serwera, a serwer rozsyła ten ciąg wraz z identyfikatorem nadawcy oraz aktualną datą do wszystkich pozostałych klientów

- 2ONE id_klienta string:

    Wysłanie wiadomości do konkretnego klienta. Klient wysyła do serwera ciąg znaków podając jako adresata konkretnego klienta o identyfikatorze z listy aktywnych klientów. Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do wskazanego klienta.

- STOP: 

    Zgłoszenie zakończenia pracy klienta.  Powinno skutkować usunięciem klienta z listy klientów przechowywanej na serwerze 

- ALIVE:

    serwer powinien cyklicznie "pingować" zarejestrowanych klientów, aby zweryfikować że wciąż odpowiadają na żądania, a jeśli nie - usuwać ich z listy klientów.

Klient przy wyłączeniu Ctrl+C powinien wyrejestrować się z serwera

Dla uproszczenia można przyjąć, że serwer przechowuje informacje o klientach w statycznej tablicy (rozmiar tablicy ogranicza liczbę klientów, którzy mogą jednocześnie byc uczestnikami czatu).

## Lab 12: Sockety - część 2
### Zadanie: Prosty chat 2
Zmodyfikuj rozwiązanie zadania z poprzedniego laboratorium w taki sposób aby komunikacja pomiędzy klientami a serwerem odbywała się z użyciem protokołu datagramowego.
