
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
    local v = self.value
    local width = v.width
	local height = v.height
	local depth = v.depth
    self.mesh = self.gen:Cube(width, height, depth);
    return (self.mesh ~= nil)
end

function CubeGenerator:MeshData()
    return self.mesh
end
