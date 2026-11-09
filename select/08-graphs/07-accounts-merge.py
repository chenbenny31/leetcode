# LC 721 Accounts Merge (Medium). Pattern: union-find over shared keys.
# accounts[i] = [name, emails...]. Two accounts belong to the same person if they share
# an email. Merge them into [name, sorted distinct emails...], in any order.

from collections import defaultdict


# returns the root account of node, compressing the path
def find_root(parent, node):
    if parent[node] != node:
        parent[node] = find_root(parent, parent[node])
    return parent[node]


# Union-find over account indices, joined through shared emails.
# T: O(E log E) for sorting, E = total emails; S: O(E).
def accounts_merge(accounts):
    n = len(accounts)
    # parent[i] = parent account of i in the union-find
    parent = list(range(n))
    # email -> first account that listed it
    owner = {}
    for i in range(n):
        for j in range(1, len(accounts[i])):
            email = accounts[i][j]
            if email not in owner:
                owner[email] = i
                continue
            root_cur = find_root(parent, i)
            root_owner = find_root(parent, owner[email])
            parent[root_cur] = root_owner
    # root account -> its distinct emails
    groups = defaultdict(list)
    for email, owner_id in owner.items():
        groups[find_root(parent, owner_id)].append(email)
    result = []
    for root, emails in groups.items():
        emails.sort()
        merged = [accounts[root][0]]
        for email in emails:
            merged.append(email)
        result.append(merged)
    return result


def test(label, accounts, want):
    got = sorted(accounts_merge(accounts))
    print(label, "PASS" if got == sorted(want) else "FAIL")


def main():
    test("chain", [["Ann", "a@x", "b@x"], ["Ann", "c@x"], ["Ann", "b@x", "c@x"]],
         [["Ann", "a@x", "b@x", "c@x"]])
    test("same-name-apart", [["Bob", "b1@x"], ["Bob", "b2@x"]],
         [["Bob", "b1@x"], ["Bob", "b2@x"]])
    test("dup-email", [["Cy", "c@x", "c@x"]], [["Cy", "c@x"]])


main()

# follow-up: DFS instead of union-find -> graph[email] = emails in the same account (link
#   the first email to the rest), DFS each component, sort; O(E log E)
# follow-up: union by size -> with path compression ~O(alpha(n)) per op
