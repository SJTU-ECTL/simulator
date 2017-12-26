# Readme

> Simulator for `blif` circuit.

> Origin Author: [Yao Yue](https://github.com/tripack45)

> Maintainer: [Su Hang](https://github.com/tonyfloatersu) and [Zhou Zhuangzhuang](https://github.com/zzhou612)

> Code view tips: if you view it on github, please add ?ts=4

## How to use

- Supported platform: Linux
- Required compiler: g++ (cpp17)
- Make system: Cmake (using clion)
- Additional requirements: boost

### Some Basic Introduction to Install and Use

- First set the `install_bnet`, edit its configuration with executable `dummy_target`.
- Then change the build target into `simu_test`.

### Circuit Convention

- Each input/output/internal node start with no-digit symbol
- Allow Symbol only from [A-Za-z0-9_], no other symbol

## TBD

- [ ] simulation diff test enable.
- [ ] BooleanNetwork version match, current one is ahead of origin version.

<div style="page-break-after: always;"></div>

## Project Structure

```mermaid
graph TD
A[exception] --> B[generator]
C[urandom] --> B[generator]
D[BooleanNetwork] --> E[convert blif to cpp]
E --> F[circuit simulation]
B --> G[simulation diff test]
F --> G
H[simulation result type] --> G
D --> H
C --> E
I[Memorize] --> D
I --> F
```
### Simulation Diff Test

```mermaid
graph LR
A[generator] -->|input| B[circuit simulation]
A[generator] -->|input| C[...]
A[generator] -->|input| D[circuit simulation]
B --> |output| E[Diff Check]
C --> |output| E[Diff Check]
D --> |output| E[Diff Check]
E --> F[Result report]
```



