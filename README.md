# Philosophers — Concurrency

Dining Philosophers problem implemented in C using threads, mutexes, and synchronization.

## 🧠 Overview

This project simulates philosophers sitting at a table, alternating between eating, sleeping, and thinking while competing for shared forks.

The main focus is correct concurrent behavior and synchronization:

- one thread per philosopher
- one mutex per fork
- synchronized output
- death monitoring
- clean shutdown when the simulation ends

## 🎮 Features

- Supports up to `200` philosophers
- Starvation detection with a dedicated monitor thread
- Stops when a philosopher dies
- Optional limit on number of meals
- Safe initialization and cleanup (rollback on failure)

## 🛠 Tech Stack

- C
- `pthread`
- `pthread_mutex_t`
- `gettimeofday`
- `usleep`

## 🏗 Architecture

- **Philosophers (threads)** — each philosopher runs in its own thread  
- **Forks (mutexes)** — shared resources protected by mutexes  
- **Monitor** — checks death conditions and stops simulation  
- **Timing system** — controls precise action timing

## 🧠 Core Concepts

- Multithreading
- Mutex synchronization
- Deadlocks and starvation
- Race conditions
- Thread lifecycle management

## ⚡ Challenges

- Preventing deadlocks when multiple threads compete for resources  
- Avoiding starvation while maintaining performance  
- Synchronizing output to prevent race conditions  
- Handling precise timing in a multithreaded environment  

## ⚙️ Build

```bash
make
```

Useful targets:

```bash
make
make clean
make fclean
make re
```

## ▶️ Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Parameters:

- `number_of_philosophers`: from `1` to `200`
- `time_to_die`: time in milliseconds before a philosopher dies without eating
- `time_to_eat`: eating duration in milliseconds
- `time_to_sleep`: sleeping duration in milliseconds
- `number_of_times_each_philosopher_must_eat`: optional limit; when reached by all philosophers, the simulation stops

## 🧪 Examples

Run without meal limit:

```bash
./philo 5 800 200 200
```

Run until each philosopher eats 3 times:

```bash
./philo 5 800 200 200 3
```

## 🖥 Output Format

The program prints timestamped status messages in the standard `philo` format:

```text
<timestamp_in_ms> <philosopher_id> has taken a fork
<timestamp_in_ms> <philosopher_id> is eating
<timestamp_in_ms> <philosopher_id> is sleeping
<timestamp_in_ms> <philosopher_id> is thinking
<timestamp_in_ms> <philosopher_id> died
```

Example:

```text
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
```

## 📁 Project Structure

```text
.
├── includes/
│   └── philo.h
├── sources/
│   ├── app/
│   ├── cleanup/
│   ├── death/
│   ├── init/
│   ├── output/
│   ├── parse/
│   ├── philo/
│   └── time/
├── Makefile
└── README.md
```

## 🧩 Implementation Details

- `sources/init/` initializes the shared table, philosophers, forks, and mutexes
- `sources/philo/` contains the philosopher routine and fork handling
- `sources/death/` contains the monitoring logic and stop conditions
- `sources/time/` handles timing and custom sleep behavior
- `sources/output/` serializes status and error output
- `sources/cleanup/` frees allocated resources and destroys mutexes safely

## 🧑‍💻 Author

- Vitali Lund — https://github.com/lundaevv