#!/bin/bash
#
# run_experiments.sh
#
# Uso: chmod +x run_experiments.sh
#       ./run_experiments.sh
#

gcc v3.c -mavx2 -mfma -lm -o3 -o v3

# 1) Define los valores de los parámetros
param1_values=(250 2500 5000)
param2_values=(1 16 64)
repetitions=15
p2=1

# 2) Bucle anidado
for p1 in "${param1_values[@]}"; do
  for run in $(seq 1 "$repetitions"); do
    echo "Ejecutando run #$run con parámetros: p1=$p1, p2=$p2"
    ./v3 "$p1" "$p2" >> v3_o3_${p1}_${p2}.log
  done
done

echo "Todas las ejecuciones han terminado."