
CubeGenerator = {}
setmetatable(CubeGenerator, {__index = HiveBaseModule})

CubeGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=CubeGenerator})
    return this
end

function CubeGenerator:Do()
    self:UpdateValue()
    return true
end

function CubeGenerator:MeshData()
    local v = self.value
    local width = v.width
	local height = v.height
	local depth = v.depth
    return self.gen:Cube(width, height, depth);
end
