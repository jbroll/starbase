#!/usr/bin/env tclsh9.0

# Initialize variables
set header 1
set in_code_block 0
set in_list 0
set in_plus_code_block 0
set prev_line ""
set prev_line_length 0

proc process_line {line} {
    global header in_code_block in_list in_plus_code_block prev_line prev_line_length

    # Skip header lines
    if {$header && [regexp {^[A-Za-z]+:} $line]} {
        return
    }
    set header 0

    # Handle code blocks
    if {$line eq "-"} {
        if {$in_code_block || $in_plus_code_block} {
            puts "```"
            set in_code_block 0
            set in_plus_code_block 0
        } else {
            puts "```"
            set in_code_block 1
        }
        return
    } 

    # Handle plus code blocks
    if {$line eq "+"} {
        if {!$in_code_block && !$in_plus_code_block} {
            puts "```"
            set in_plus_code_block 1
        }
        return
    }

    if {[regexp {^include (.*)$} $line -> file]} {
        set file [string map { . - } $file].md
        puts "{% include $file %}"
        return
    }
	
    # Convert list items
    if {[regexp {^ \* } $line]} {
        if {!$in_list} {
            puts ""
            set in_list 1
        }
        regsub {^ \* } $line "- " line
    } else {
			# End list if line doesn't start with ' * '
			if {![regexp {^[ *]} $line]} {
				if {$in_list} {
					puts ""
					set in_list 0
				}
			}
	}

    # Convert links
    set line [regsub {@{([^\}]+),([^\}]+)}} $line {[\1](\2)}]
    set line [regsub {@\[([^]]+),([^]]+)\]} $line {[\1](\2)}]
    set line [regsub -all { @\[([^ \t]+)\]} $line {[\1](\1.html)}]
    set line [regsub -all { @([^ \t]+)} $line { [\1](\1.html)}]
    set line [string map { { index.html} { index-cmd.html} { starbase.html} { index.html} } $line] 

    # Convert inline formatting
    if {!$in_code_block} {
        set line [regsub -all {\*\*\*([^*]+)\*\*\*} $line {**\1**}]
        set line [regsub -all {~([^~]+)~} $line {*\1*}]
        set line [regsub -all {#([^ ]+)} $line {`\1`}]

    }

    # Print the processed line
    if {$in_code_block || $in_plus_code_block} {
        puts $line
    } else {
        puts $line
    }

    set prev_line $line
    set prev_line_length [string length $line]
}

while {[gets stdin line] >= 0} {
    process_line $line
}
