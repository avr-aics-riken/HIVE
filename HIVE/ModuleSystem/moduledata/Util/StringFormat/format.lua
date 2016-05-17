StringFormat = {}
setmetatable(StringFormat, {__index = HiveBaseModule})

StringFormat.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.property = {
        outstr = ""
    }
    setmetatable(this, {__index=StringFormat})
    return this
end

function StringFormat:Do()
    self:UpdateValue()
    local v = self.value
    print (v.input)
    print (v.format)
	local fmt = v.format
	local inputlist = {}
	local cnt = 1
	function unpack (t, i)
		i = i or 1
		if t[i] ~= nil then
			return t[i], unpack(t, i + 1)
		end
	end
    if v.input then
        for i,v in pairs(v.input) do -- TODO: Order
            inputlist[cnt] = v
            cnt = cnt + 1
        end
    end

	local outstr = ''
	if #inputlist > 0 then
		self.outstr = string.format(fmt, unpack(inputlist))
	end
    
    return true
end

function StringFormat:String()
    print(self.outstr)
    return self.outstr
end
