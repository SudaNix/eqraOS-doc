; **************************************************
;	cluster_to_lba: convert cluster number to LBA
;   input:
;				ax: Cluster number.
;	output:
;				ax: lba number.
; **************************************************
cluster_to_lba:
		
		; lba = (cluster - 2)* sectors_per_cluster
		; the first cluster is always 2.
		
		sub ax,2
		
		xor cx,cx
		mov cl, byte[sectors_per_cluster]
		mul cx
		
		add ax,word[data_region]		; cluster start from data area.
		ret
