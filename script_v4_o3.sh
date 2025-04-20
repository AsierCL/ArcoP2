#!/bin/bash
#
# run_experiments.sh
#
# Uso: chmod +x run_experiments.sh
#       ./run_experiments.sh
#

gcc v4.c -lm -fopenmp -o3 -o v4

# 1) Define los valores de los parámetros
param1_values=(250 2500 5000)
param2_values=(1 4 16 64)
repetitions=10

# 2) Bucle anidado
for p1 in "${param1_values[@]}"; do
  for p2 in "${param2_values[@]}"; do
    for run in $(seq 1 "$repetitions"); do
      echo "Ejecutando run #$run con parámetros: p1=$p1, p2=$p2"
      ./v4 "$p1" "$p2" >> v4_o3_${p1}_${p2}.log
    done
  done
done

echo "Todas las ejecuciones han terminado."
