# pr_tpp8 — Варіант 9 (Система моніторингу серверів)

Сутності: Server (основна) та Metric (залежна), зв'язок 1:N через Metric.serverId -> Server.id.

## Збирання (Windows, Visual Studio 2022+ / CMake)

1. Встановити Visual Studio з робочим навантаженням «Розробка класичних застосунків на C++».
2. Встановити vcpkg, наприклад у C:\vcpkg:
       git clone https://github.com/microsoft/vcpkg C:\vcpkg
       C:\vcpkg\bootstrap-vcpkg.bat
3. Задати змінну середовища VCPKG_ROOT = C:\vcpkg і перезапустити Visual Studio.
4. У каталозі проєкту виконати:
       C:\vcpkg\vcpkg.exe x-update-baseline --add-initial-baseline
5. Відкрити папку проєкту у Visual Studio (File -> Open -> Folder) або виконати з «x64 Native Tools Command Prompt»:
       cmake --preset default
       cmake --build build
       build\pr_tpp8.exe

## Очікуваний результат

- Після підготовки даних: 5 серверів / 15 метрик.
- Після COMMIT: 6 / 16. Після ROLLBACK кількості не змінюються.
- У server_no_transaction.db без транзакції залишається 1 сервер і 0 метрик.
