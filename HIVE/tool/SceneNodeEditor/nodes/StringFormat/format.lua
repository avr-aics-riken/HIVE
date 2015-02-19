function StringFormat(arg)
	local fmt = arg.format
	local inputlist = {}
	local cnt = 1
	function unpack (t, i)
		i = i or 1
		if t[i] ~= nil then
			return t[i], unpack(t, i + 1)
		end
	end
	for i,v in pairs(arg.format.input) do -- TODO: Order
		inputlist[cnt] = v
		cnt = cnt + 1
	end
	local outstr = string.format(fmt, unpack(inputlist))
	return {
		String = function()
			return outstr
		end
	}
end