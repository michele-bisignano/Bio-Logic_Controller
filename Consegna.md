# 🧪 Controllore per Camera di Fermentazione di Precisione  
### *Architettura a Oggetti e Logica Predittiva*

---

## 🎯 Obiettivo Generale

Sviluppare un **firmware completo** per un **sistema embedded basato su Arduino**, in grado di gestire con precisione una **camera di fermentazione**. Il progetto deve dimostrare competenze avanzate di programmazione, tra cui:

- Implementazione di un'**architettura orientata agli oggetti (OOP)**
- Gestione di problematiche hardware reali (es. **debouncing dei pulsanti**)
- Uso di **interrupt** per eventi critici
- Integrazione di una **logica di controllo predittiva** per migliorare prestazioni e stabilità

---

## 🛠️ Architettura Hardware *(da realizzare su Autodesk Tinkercad)*

Il circuito deve essere assemblato utilizzando **Arduino Uno R3** e includere i seguenti componenti:

| Componente                    | Funzione                                                                 |
|------------------------------|--------------------------------------------------------------------------|
| **Sensore di Temperatura TMP36** | Misura con precisione la temperatura interna della camera                |
| **Sensore di Gas MQ-3**          | Simula il monitoraggio dell’etanolo (sottoprodotto della fermentazione)  |
| **Display LCD 16x2 con I2C**     | Visualizza lo stato del sistema, dati sensoriali e impostazioni          |
| **Potenziometro**                | Permette di impostare dinamicamente il setpoint di temperatura           |
| **Pulsante di Navigazione**      | Cicla tra le schermate informative sul display                           |
| **Relè a 1 Canale**              | Controlla un elemento riscaldante (motore DC o resistenza)               |
| **LED Bicolore (o rosso/verde)** | Fornisce un'indicazione visiva immediata dello stato operativo           |
| **Pulsante di Arresto d’Emergenza** | Ferma il sistema in caso di evento critico tramite interrupt             |

---

## 🧩 Architettura Software e Requisiti Funzionali

### 🧱 Struttura a Oggetti (OOP)

Il firmware deve seguire un’architettura **Object-Oriented**, con responsabilità separate in file `.h` e `.cpp`. Devono essere implementate almeno le seguenti classi:

- `SensorManager`: Gestione delle letture da sensori
- `ActuatorController`: Controllo del relè e del LED
- `DisplayManager`: Interazione con il display LCD
- `DebouncedButton`: Gestione del pulsante con logica di debouncing tramite `millis()`
- `SystemState`: Logica principale e gestione della macchina a stati

---

### 🔘 Gestione Input con Debouncing

- Il **pulsante di navigazione** deve essere gestito tramite **debouncing software**
- La classe `DebouncedButton` deve garantire che ogni pressione venga letta **una sola volta**, utilizzando la funzione `millis()`

---

### 🚨 Gestione Interrupt

- Il **pulsante di emergenza** deve essere collegato al **pin 2 o 3** (interrupt esterno) di Arduino
- Alla pressione deve:
  - Attivare una **Interrupt Service Routine (ISR)**
  - **Disattivare immediatamente** il riscaldamento
  - Portare il sistema in uno **stato sicuro**, a prescindere dallo stato corrente

---

### 🔁 Macchina a Stati (FSM)

Il sistema deve operare secondo una **macchina a stati finiti**, comprendente almeno:

| Stato               | Descrizione                                                       |
|---------------------|--------------------------------------------------------------------|
| `STANDBY`           | Stato di attesa in attesa di comandi                              |
| `PREHEATING`        | Riscaldamento iniziale fino al raggiungimento del setpoint        |
| `MAINTAINING`       | Mantenimento della temperatura con logica predittiva              |
| `EMERGENCY_STOP`    | Attivato tramite interrupt; arresto immediato delle operazioni     |

---

### 🔮 Logica di Controllo Predittiva

Durante lo stato `MAINTAINING` il sistema deve:

- Non aspettare che la temperatura scenda sotto la soglia
- Analizzare la **velocità di raffreddamento** (derivata prima della temperatura nel tempo)
- Attivare proattivamente il riscaldatore con **brevi impulsi**
- Migliorare la **stabilità termica** e ridurre il **consumo energetico**

---

### 📺 Interfaccia Utente su LCD

Il display LCD deve fornire un feedback chiaro, mostrando:

- Lo **stato operativo attuale**
- La **temperatura attuale**
- Il **setpoint** impostato con il potenziometro
- Messaggi come:
  - `"Riscaldamento..."`
  - `"Temperatura Stabile"`
  - `"ARRESTO DI EMERGENZA"`

---

## 🧪 Piattaforma di Sviluppo e Test

L’intero progetto (circuito + firmware) deve essere sviluppato, assemblato e testato **esclusivamente sulla piattaforma Autodesk Tinkercad**.

---

