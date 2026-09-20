#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = N / size;

    int *array = NULL;

    if (rank == 0) {
        array = (int *)malloc(N * sizeof(int));

        for (int i = 0; i < N; i++)
            array[i] = i + 1;

        printf("Root filled array with values 1 to %d\n", N);
    }

    int *local_chunk =
        (int *)malloc(chunk_size * sizeof(int));

    double start = MPI_Wtime();

    MPI_Scatter(
        array,
        chunk_size,
        MPI_INT,
        local_chunk,
        chunk_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    long long local_sum = 0;

    for (int i = 0; i < chunk_size; i++)
        local_sum += local_chunk[i];

    /*
     * MPI_Scan computes a prefix sum.
     *
     * Rank 0 -> local_sum of rank 0
     * Rank 1 -> rank 0 + rank 1
     * Rank 2 -> rank 0 + rank 1 + rank 2
     * ...
     */

    long long prefix_sum = 0;

    MPI_Scan(
        &local_sum,
        &prefix_sum,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        MPI_COMM_WORLD
    );

    /*
     * Sum of all chunks before this rank.
     */
    long long sum_before_me =
        prefix_sum - local_sum;

    /*
     * Verification using:
     *
     * Sum(1 ... K) = K * (K + 1) / 2
     *
     * where K = (rank + 1) * chunk_size
     */
    long long K =
        (long long)(rank + 1) * chunk_size;

    long long expected_prefix =
        K * (K + 1) / 2;

    int prefix_correct =
        (prefix_sum == expected_prefix);

    printf(
        "  Rank %d: local_sum = %lld, "
        "prefix_sum = %lld, "
        "sum_before_me = %lld, "
        "prefix_correct = %s\n",
        rank,
        local_sum,
        prefix_sum,
        sum_before_me,
        prefix_correct ? "YES" : "NO"
    );

    /*
     * The last rank should have the global total.
     */
    if (rank == size - 1) {

        double elapsed =
            MPI_Wtime() - start;

        long long expected_global =
            (long long)N * (N + 1) / 2;

        printf("\n[Scan] Last rank prefix sum = %lld\n",
               prefix_sum);

        printf("[Scan] Expected global total = %lld\n",
               expected_global);

        printf("[Scan] Correct? = %s\n",
               prefix_sum == expected_global
                   ? "YES"
                   : "NO");

        printf("[Scan] Time = %.4f sec\n",
               elapsed);
    }

    free(local_chunk);

    if (rank == 0) {
        free(array);
    }

    MPI_Finalize();

    return 0;
}
