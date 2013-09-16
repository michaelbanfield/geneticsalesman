#/bin/bash
#PBS -N darcy_mpi
#PBS -A uq-COSC3500
#PBS -lwalltime=00:05:00
#PBS -lselect=2:ncpus=8:mpiprocs=4:mem=1gb

. /usr/share/modules/init/bash
module load intel-mpi
module load intel-cc-11
cd $PBS_O_WORKDIR
# start the MPI daemons:
mpdboot -f ${PBS_NODEFILE} -r ssh -n 4
# execute program:
export OMP_NUM_THREADS=2
mpiexec -envlist OMP_NUM_THREADS -np 3 ./bin/geneticsalesman 1000file.map 100 1000 10
# shut down MPI daemons:
mpdallexit


