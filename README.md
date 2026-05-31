### How to use:
1. Compile (llvm + cpp20), requires -insecure launch option in CS:GO
2. Place `0x221B0000.bin` (built monolith binary) next to `steam_module.dll`
3. Inject `steam_module.dll` into the parent steam.exe process — it will auto-inject the cheat when CS:GO launches
4. Launch `local_server.exe` (port 1488, configs stored in `configs/` next to the exe)
5. Inject `memelith.dll` into csgo.exe

### Cloud
Local server uses TCP on port 1488. The TCP wire protocol is implemented in monolith — no extra setup needed.

### Credits:
https://www.youtube.com/watch?v=TD5Qjssu2MM

## License

```
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2022 4wi <n1_4eJL@cringe.solutions>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
```
