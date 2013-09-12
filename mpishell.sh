!/bin/bash
#PBS -N mpiOmpTest
#PBS -A uq-COSC3500
#PBS -q S660004
#PBS -l select=4:ncpus=8:mem=30GB
#PBS -l walltime=00:01:00
#PBS -l place=excl
# load modules:
. /usr/share/modules/init/bash
module load intel-mpi
module load intel-cc-11
cd $PBS_O_WORKDIR
# start the MPI daemons:
mpdboot -f ${PBS_NODEFILE} -r ssh -n 4
# execute program:
export OMP_NUM_THREADS=8
mpiexec -envlist OMP_NUM_THREADS -np 4 ./bin/geneticsalesman 1000file.map 100 1000 10
# shut down MPI daemons:
mpdallexit
