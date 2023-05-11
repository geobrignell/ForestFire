#!/bin/bash

#SBATCH --job-name=forest_fire
#SBATCH --partition=teach_cpu
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=27
#SBATCH --time=0:5:00
#SBATCH --mem-per-cpu=100M

## Direct output to the following files.
## (The %j is replaced by the job id.)
#SBATCH -e forest_fire_err_%j.txt
#SBATCH -o forest_fire_out_%j.txt

# Just in case this is not loaded already...
module load languages/intel/2020-u4

# Change to working directory, where the job was submitted from.
cd "${SLURM_SUBMIT_DIR}" 

# Record some potentially useful details about the job: 
echo "Running on host $(hostname)"
echo "Started on $(date)"
echo "Directory is $(pwd)"
echo "Slurm job ID is ${SLURM_JOBID}"
echo "This jobs runs on the following machines:"
echo "${SLURM_JOB_NODELIST}" 
printf "\n\n"

# Submit
srun --mpi=pmi2 ./forfire 100 0.5

# Output the end time
printf "\n\n"
echo "Ended on: $(date)"