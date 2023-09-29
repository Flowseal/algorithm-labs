def rec(m: list) -> int:
  stack = []
  res = 0

  for i in m:
    if i in stack:
      c = 0

      while i != stack[-1]:
        c += 1
        stack.pop()

      if c == 0:
        print("ERROR 1")
        return
      
      if c > 1:
        print("ERROR 2")
        return
    else:
      stack.append(i)
    res = max(res, sum(stack))

  return res

print(rec([1, 2, 3, 2, 1, 4, 1, 5, 1, 6, 1]))