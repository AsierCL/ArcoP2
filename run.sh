#!/bin/bash
#SBATCH --job-name=experimentos_C
#SBATCH --output=experimentos_%j.out
#SBATCH --error=experimentos_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --mem=64G
#SBATCH --time=01:00:00
#SBATCH -p normal

# Ejecuta el script de Bash con los bucles
bash script_v1_o0.sh