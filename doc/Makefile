
lib:
	c2man -Swu -I. -I/data/mmti/include -P "cpp -D__STDC__ -E -C" ../tablelib/table.h
	wu set Outline starbase.3.wu table_*.wu
	wu 2html table_*.wu
	cat starbase.3.ABC.head  > starbase.3.ABC.wu
	awk '$$2 ~ /^-/ { $$1 = " * @{"$$1", "$$1".html}"; print }' table_*.wu >> starbase.3.ABC.wu
	cat starbase.3.ABC.tail >> starbase.3.ABC.wu
	wu 2html starbase.3.ABC.wu

ABC:
	mawk '/^NAME/,/^SYNOPSYS/ { if ( $$2 ~ /^-$$/ ) { $$1 = " * @["$$1", "$$1".html]"; print } }' *.wu | grep -v table_ | sed -e s/#//g > xxABC

