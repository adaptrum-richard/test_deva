
function popen_read2()
	local cmd = "ps -ax";
	local fp = io.popen(cmd);
	local tmp = '';
	if fp then
		tmp = fp:read();
		fp:close();
		return 1;
	end
	return -1;
end


result = popen_read2();
