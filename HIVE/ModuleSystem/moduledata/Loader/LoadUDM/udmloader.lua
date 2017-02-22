

function LoadUDM(property)
	local udm = require("UdmLoader")()
	udm:Load(property.filepath)
	local udminst = {
		Tetra = function()
			return udm:TetraData()
		end,
		Mesh = function ()
			return udm:MeshData()
		end,
		ExtraP = function ()
			return udm:ExtraData('P')
		end,
		ExtraT = function ()
			return udm:ExtraData('TEMPARATURE')
		end,
		ExtraU = function ()
			return udm:ExtraData('U')
		end,
		ExtraV = function ()
			return udm:ExtraData('V')
		end,
		ExtraW = function ()
			return udm:ExtraData('W')
		end
	}
	return udminst
end


LoadUDM = {}
setmetatable(LoadUDM, {__index = HiveBaseModule})

LoadUDM.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = require('UdmLoader')()
    setmetatable(this, {__index=LoadUDM})
    return this
end

function LoadUDM:Do()
    self:UpdateValue()
    return self.loader:Load(self.value.filepath, self.value.step)
end

function LoadUDM:Tetra()
    return self.loader:TetraData()
end

function LoadUDM:Mesh()
    return self.loader:MeshData()
end

function LoadUDM:Solid()
    return self.loader:SolidData(this.value.solidtype)
end

function LoadUDM:ExtraData(index)
    local name = self.value.extraname[index + 1]
    return self.loader:ExtraData(name)
end
