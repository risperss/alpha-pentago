import pentago

import numpy as np


def niceprint(x):
    s = ""
    for (index, item) in enumerate(x):
        s += f"\t{item:0.1f} "

        if index % 6 == 5:
            s += "\n"

    print(s)


def purecmaes():
    # User defined input parameters (need to be edited)
    N = pentago.NUM_WEIGHTS  # number of objective variables/problem dimension
    xmean = np.random.rand(N) # objective variables initial point
    sigma = 0.05  # coordinate wise standard deviation (step size)
    stopfitness = 1e-14  # stop if fitness < stopfitness (minimization)
    stopeval = int(1e3 * N**2)  # stop after stopeval number of function evaluations

    # Strategy parameter setting: Selection
    lambda_ = 4 + int(3 * np.log(N))  # population size, offspring number
    mu = lambda_ // 2  # number of parents/points for recombination
    weights = np.log(mu + 0.5) - np.log(
        np.arange(1, mu + 1)
    )  # muXone array for weighted recombination
    mu = int(mu)
    weights = weights / sum(weights)  # normalize recombination weights array
    mueff = (sum(weights) ** 2) / sum(
        weights**2
    )  # variance-effectiveness of sum w_i x_i

    # Strategy parameter setting: Adaptation
    cc = (4 + mueff / N) / (N + 4 + 2 * mueff / N)  # time constant for cumulation for C
    cs = (mueff + 2) / (N + mueff + 5)  # t-const for cumulation for sigma control
    c1 = 2 / ((N + 1.3) ** 2 + mueff)  # learning rate for rank-one update of C
    cmu = min(
        1 - c1, 2 * (mueff - 2 + 1 / mueff) / ((N + 2) ** 2 + mueff)
    )  # and for rank-mu update
    damps = 1 + 2 * max(0, np.sqrt((mueff - 1) / (N + 1)) - 1) + cs  # damping for sigma
    # usually close to 1
    # Initialize dynamic (internal) strategy parameters and constants
    pc = np.zeros(N)  # evolution paths for C
    ps = np.zeros(N)  # evolution paths for sigma
    B = np.eye(N, N)  # B defines the coordinate system
    D = np.ones(N)  # diagonal D defines the scaling
    C = B @ np.diag(D**2) @ B.T  # covariance matrix C
    invsqrtC = B @ np.diag(D**-1) @ B.T  # C^-1/2
    eigeneval = 0  # track update of B and D
    chiN = N**0.5 * (1 - 1 / (4 * N) + 1 / (21 * N**2))  # expectation of
    #   ||N(0,I)|| == norm(randn(N,1))
    # Generation Loop
    counteval = 0  # the next 40 lines contain the 20 lines of interesting code

    while counteval < stopeval:
        # Generate and evaluate lambda offspring
        arx = np.zeros((N, lambda_))
        arfitness = np.zeros(lambda_)
        for k in range(lambda_):
            arx[:, k] = xmean + sigma * B @ (
                D * np.random.randn(N)
            )  # m + sig * Normal(0,C)
            counteval += 1

        arfitness = pentago.fitnesses(arx.transpose().tolist())

        # Sort by fitness and compute weighted mean into xmean
        arfitness = np.sort(arfitness)  # minimization
        arindex = np.argsort(arfitness)
        xold = xmean
        xmean = arx[:, arindex[:mu]] @ weights  # recombination, new mean value

        # Cumulation: Update evolution paths
        ps = (1 - cs) * ps + np.sqrt(cs * (2 - cs) * mueff) * invsqrtC @ (
            xmean - xold
        ) / sigma
        hsig = (
            np.linalg.norm(ps)
            / np.sqrt(1 - (1 - cs) ** (2 * counteval / lambda_))
            / chiN
        )
        if hsig < 1.4 + 2 / (N + 1):
            # Adapt covariance matrix C
            pc = (1 - cc) * pc + hsig * np.sqrt(cc * (2 - cc) * mueff) * (
                xmean - xold
            ) / sigma
            # Adapt step size sigma
            sigma = sigma * np.exp((cs / damps) * (np.linalg.norm(ps) / chiN - 1))
            # Update B and D from C
            C = (1 - c1 - cmu) * C + c1 * (pc @ pc.T + (1 - hsig) * cc * (2 - cc) * C)
            for i in range(N):
                C[i, i] = max(C[i, i], 0)
            eigeneval += 1
            # Decomposition of C into B*diag(D.^2)*B' (diagonalization)
            if eigeneval > (N + lambda_) / (2 * cs / (2 + cs)):
                eigeneval = 0
                B, D, invsqrtC = np.linalg.svd(C)
                D = np.sqrt(D)
                invsqrtC = B @ np.diag(D**-1) @ B.T

        # Break, if fitness is good enough or condition exceeds 1e14, better termination methods are advisable
        if arfitness[0] <= stopfitness or np.max(D) > 1e7 * np.min(D):
            break

        print(arx[:, arindex[0]])

    # Return best point of last iteration.
    # Notice that xmean is expected to be even better.
    xmin = arx[:, arindex[0]]
    return xmin

x = purecmaes()

print(x)
