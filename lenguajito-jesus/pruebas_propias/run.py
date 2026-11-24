#!/usr/bin/env python3
"""Pruebas independientes; no modifica las pruebas doradas."""
import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
BIN = ROOT / 'lenguajito'
total = 0

def test(source, mode, code=0, expected=None, contains=()):
    global total
    with tempfile.TemporaryDirectory(prefix='lj pruebas ') as directory:
        path = pathlib.Path(directory) / 'programa con espacios.lj'
        path.write_text(source, encoding='utf-8')
        r = subprocess.run([str(BIN), str(path), mode], capture_output=True,
                           text=True, timeout=10)
        assert r.returncode == code, (source, r.returncode, r.stdout, r.stderr)
        if expected is not None:
            assert r.stdout == expected, (source, r.stdout, expected)
        for text in contains:
            assert text in r.stdout, (source, text, r.stdout)
        total += 1

test('int x; if (1) { x=1; } print(x);', '--check', 1, contains=['E01'])
test('int x; if (1) { x=1; } else { x=2; } print(x);', '--run', expected='1\n')
test('int x; while (0) { x=1; } print(x);', '--check', 1, contains=['E01'])
test('int x=2; { int x=5; print(x); } print(x);', '--run', expected='5\n2\n')
test('int x=2; { int x=x; }', '--check', 1, contains=['E01'])
test('{ int x=2; } print(x);', '--check', 1, contains=['E01'])
test('int x=1; int x=2; print(y); print(10/(3-3));', '--check', 1,
     contains=['E02','E01','E04'])
test('int x=;\nint y=;\n', '--check', 1, contains=[':1:', ':2:'])
test('if (1) print(1);', '--check', 1, contains=['S01'])
test('print(1<2<3);', '--check', 1, contains=['S01'])
test('print(--1);', '--check', 1, contains=['S01'])
test('print(1!2);', '--tokens', 1, contains=['S01'])
test('// comentario\nint x=2;\n', '--tokens', contains=['2:1 KW int','2:5 ID x','3:1 EOF'])
test('print(2*3+4);', '--tac', expected='print 10\n')
test('print(-7/2); int x=-7; int y=2; print(x/y);', '--run', expected='-3\n-3\n')
test('print(2147483647+1); int x=2147483647; print(x+1);', '--run',
     expected='-2147483648\n-2147483648\n')
test('int x=-2147483648; int y=-1; print(x/y); print(-2147483648/-1);',
     '--run', expected='-2147483648\n-2147483648\n')
test('int _t0=7; print(_t0+2);', '--run', expected='9\n')
test('int x=3; print(x<4); print(x<=3); print(x>4); print(x>=3); print(x==3); print(x!=3);',
     '--run', expected='1\n1\n0\n1\n1\n0\n')
test('int i=0; int s=0; while(i<4){ int j=0; while(j<2){ s=s+i; j=j+1; } i=i+1; } print(s);',
     '--run', expected='12\n')
test('', '--run', expected='')
test('\n'.join('print(x);' for _ in range(30)), '--check', 1)
for args in ([], ['missing.lj', '--desconocido']):
    r = subprocess.run([str(BIN), *args], capture_output=True)
    assert r.returncode == 2
print(f'Propias: {total + 2}/{total + 2} pruebas superadas')
