# PseudoPTY

PseudoPTY is a native Windows pseudo-terminal project written in C.

---

## Installation

Install via npm:

```bash
npm install @izhank216/pseudopty
```

**Or with yarn:**
```bash
yarn add @izhank216/pseudopty
```

## Usage (CLI)
```bat
pty.exe --terminal cmd.exe
```

## Usage (ESM)
```javascript
import PseudoPTY from "@izhank216/pseudopty";

const pty = PseudoPTY.PseudoPTY_Create("cmd.exe", 80, 25);
```

## Usage (CJS) 
```javascript
const PseudoPTY = require("@izhank216/pseudopty");

const pty = PseudoPTY.PseudoPTY_Create("cmd.exe", 80, 25);
```
