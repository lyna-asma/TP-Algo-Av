import random

# ---------- Tools ----------
def sieve(limit):
    """All primes <= limit (Sieve of Eratosthenes)."""
    is_p = [True] * (limit + 1)
    is_p[0] = is_p[1] = False
    for i in range(2, int(limit ** 0.5) + 1):
        if is_p[i]:
            for j in range(i * i, limit + 1, i):
                is_p[j] = False
    return [i for i, p in enumerate(is_p) if p]

def is_prime(n):
    """Miller-Rabin, deterministic for n < 3.3e24 (fine for 12 digits)."""
    if n < 2:
        return False
    bases = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]
    for p in bases:
        if n % p == 0:
            return n == p
    d, r = n - 1, 0
    while d % 2 == 0:          # write n-1 = d * 2^r with d odd
        d //= 2
        r += 1
    for a in bases:
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False       # a proves n is composite
    return True

def random_prime_k_digits(k):
    """One random prime written on exactly k digits."""
    lo, hi = 10 ** (k - 1), 10 ** k - 1
    while True:
        n = random.randint(lo, hi) | 1     # force odd
        if is_prime(n):
            return n

def k_digit_primes(k, count):
    """'count' distinct random primes with k digits."""
    res = set()
    while len(res) < count:
        res.add(random_prime_k_digits(k))
    return sorted(res)

def save(name, data):
    with open(f"{name}.txt", "w") as f:
        f.write("\n".join(map(str, data)))
    print(f"{name}: {len(data)} numbers, first 5 -> {data[:5]}")

# ---------- Datasets ----------
if __name__ == "__main__":
    random.seed(42)
    primes = sieve(1_000_000)                      # 78,498 primes

    # Q1 and Q2
    random100  = random.sample(primes, 100)
    random1000 = random.sample(primes, 1000)

    # Q3: 100 primes in [1000, 1_000_000]
    in_range = [p for p in primes if 1000 <= p <= 1_000_000]
    test1 = random.sample(in_range, 100)

    # Q4: 10 primes for each digit count from 3 to 12
    test2 = []
    for k in range(3, 13):
        test2 += k_digit_primes(k, 10)

    # Q5: structured sample, 10 primes with 3, 6, 9 and 12 digits
    test3 = []
    for k in (3, 6, 9, 12):
        test3 += k_digit_primes(k, 10)

    assert all(is_prime(p) for p in random100 + random1000 + test1 + test2 + test3)

    save("Random100", random100)
    save("Random1000", random1000)
    save("Test-1", test1)
    save("Test-2", test2)
    save("Test-3", test3)