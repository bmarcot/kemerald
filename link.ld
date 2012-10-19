ENTRY(__main)
phys    = 0x9000;
video   = 0xb8000;
buffer  = 0x100000;
bitmap = 0xf00000;
pdptr_table = 0xe00000;

SECTIONS
{
	.entry phys : AT(phys)
	{
		code = .;
	       	*(.entry)
    		/*. = ALIGN(4096);*/
	}
	.text : AT(phys + (text - code))
	{
		text = .;
	       	*(.text)
    		. = ALIGN(4096);
	}
	.data : AT(phys + (data - code))
	{
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	.bss : AT(phys + (bss - code))
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

/*
	.text phys : AT(phys)
	{
		code = .;
	       	*(.text)
    		. = ALIGN(4096);
	}
	.data : AT(phys + (data - code))
	{
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	.bss : AT(phys + (bss - code))
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
*/
	.console_video video : AT(video)
	{
		console_video = .;
		*(console_video)
		/*. = ALIGN(4096);*/
	}
	.filedes buffer : AT(buffer)
	{
		filedes = .;
		*(.filedes)
		/*. = ALIGN(4096);*/
	}
	.phys_bitmap bitmap : AT(bitmap)
	{
		phys_bitmap = .;
		*(.phys_bitmap)
		/*. = ALIGN(4096);*/
	}
	.pdptr pdptr_table : AT(pdptr_table)
	{
		pdptr = .;
		*(.pdptr)
		/*. = ALIGN(4096);*/
	}

	end = .;
}