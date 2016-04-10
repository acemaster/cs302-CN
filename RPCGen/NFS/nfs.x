struct prog{
	string progname<255>;
};

program NFS_PROG{
	version NFS_VERS{
		string RUN_PROG(prog)=1;
	}=1;
}=0x23451111;