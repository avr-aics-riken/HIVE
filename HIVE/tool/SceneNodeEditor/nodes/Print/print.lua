function Print(arg)
	if (arg.String) then
		print('[' .. arg.Label ..']:', arg.String)
	end
end