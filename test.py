from libtheta import moments_from_samples
from scipy import random, linalg, eye


def main():
    N = 10000
    D = 10

    mean, covar = moments_from_samples(random.rand(N), random.randn(D, N))

    print("residual norm^2:",
          linalg.norm(mean) ** 2,
          linalg.norm(covar - eye(D)) ** 2)
