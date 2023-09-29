max_size = 0

def size(f) -> int:
  return 1

def rec(m: list, ram_sum: int):
  global max_size

  if ram_sum == -1:
    return -1

  if len(m) == 0:
    return ram_sum

  head = m[0]
  print(head, m)

  ram_sum += size(head)
  max_size = max(max_size, ram_sum)

  if len(m) <= 1:
    return ram_sum

  if m[0] != m[-1]:
    return -1

  i = 0

  while i < len(m):
    i += 1
    sub_calls = []
    
    if m[i] == m[i - 1]:
      print("WRONG STRUCTURE")
      return

    while i < len(m) and m[i] != head:
      sub_calls += [m[i]]
      i += 1

    if rec(sub_calls, ram_sum) == -1:
      print("WRONG STRUCTURE")
      return

rec(["main", "sort", "first", "sort", "main", "add", "main", "delete", "main", "all_delete", "main"], 0)
print("Max size", max_size)