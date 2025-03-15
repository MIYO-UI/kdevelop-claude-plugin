# KDevelop Claude Plugin

Wtyczka integrująca Claude AI z edytorem KDevelop, umożliwiająca logowanie się za pomocą konta Google.

## Funkcje

- Integracja z Claude AI bezpośrednio w KDevelop
- Logowanie za pomocą konta Google
- Wysyłanie zapytań do Claude AI
- Wsparcie dla zaznaczonego tekstu/kodu
- Wstawianie odpowiedzi Claude bezpośrednio do edytora

## Instalacja

### Wymagania
- KDevelop 5.x
- Qt 5.15+
- KDE Frameworks 5.60+

### Kompilacja

```bash
mkdir build
cd build
cmake ..
make
make install
```

### Konfiguracja

1. Otwórz KDevelop
2. Przejdź do: Ustawienia -> Konfiguruj KDevelop -> Claude AI
3. Wprowadź swój klucz API Claude
4. Wybierz model Claude

## Użycie

1. Otwórz panel Claude AI (Widok -> Panele narzędziowe -> Claude AI)
2. Zaloguj się za pomocą konta Google
3. Teraz możesz wysyłać zapytania do Claude AI

- Możesz zaznaczyć kod w edytorze i kliknąć przycisk "Send" w panelu Claude AI aby wysłać zaznaczony kod
- Użyj skrótu Ctrl+Alt+C aby szybko wysłać zaznaczony kod do Claude z kontekstem "Help me understand this code"

## Licencja

GPL v3