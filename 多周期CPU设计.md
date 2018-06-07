多周期CPU设计

#### MIPS指令格式

- R类型指令：
- I类型指令：
- J类型指令：

#### MIPS指令

- add rd, rs, rt
- sub rd, rs, rt
- and rd, rs, rt
- or    rd, rs, rt
- xor  rd, rs, rt
- sll    rd, rt, sa
- srl    rd, rt, sa
- sra   rd, rt, sa
- jr      rs
- addi  rt, rs, imm
- andi rt, rs, imm 
- ori rt, rs, imm 
- xori rt, rs, imm  
- lw rt, imm(rs) 
- sw rt, imm(rs) 
- beq rs, rt, imm 
- bne rs, rt, imm 
- lui rt, imm 
- j addr 

#### 多周期CPU

- 在多周期CPU中，指令的每个执行步骤用一个时钟周期。
  - ***IF***: Instruction Fetch
  - ***ID*** :Instruction Decode
  - ***EXE*** : Execution
  - ***MEM*** : Memory access
  - ***WB*** : Write Back
- 多周期的实现，有限状态机：
- ![屏幕截图(91)](C:\Users\DELL\Pictures\Screenshots\屏幕截图(91).png)
- 状态转移图：![屏幕截图(89)](C:\Users\DELL\Pictures\Screenshots\屏幕截图(89).png)

#### Datapath(数据通路)

- 指令与数据公用一个存储器模块
- 增加指令寄存器IR与数据寄存器DR
- PC加4是由转移地址与ALU计算
- 增加三个寄存器A,B和C

#### 各指令在各状态的操作

##### IF状态

```assembly
IR = Memory[PC]
PC = PC + 4
```

##### ID状态

```ass
PC = PC[31...28], addr << 2
```

```ass
PC = Reg[rs]
```

```ass
A = Reg[rs]
B = Reg[rt]
C = PC + (sign)imm << 2
```

##### EAL状态

```ass
C = B << sa
```

```ass
C = A op B
```

##### EAL状态

```ass
C = imm << 16
```

```ass
C = A + (sign)imm
C = A and (zero)imm
C = A or  (zero)imm
C = A xor (zero)imm
```

##### EBR状态

```ass
if (A == B) PC = C
if (A != B) PC = C
```

##### ELS状态

```ass
C = A + (sign)imm
```

##### MST状态

```ass
memory[C] = B
```

##### MLD状态

```ass
DR = memory[C]
```

##### WLD状态

```ass
Reg[rt] = DR
```

##### WAL状态

```ass
Reg[rt] = C
```

```ass
Reg[rd] = C
```

