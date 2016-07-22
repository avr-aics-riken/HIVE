ParallelCoordinate = {}
setmetatable(ParallelCoordinate, {__index = HiveBaseModule})

ParallelCoordinate.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordinate})
    this.json = 'default'
    return this
end

function ParallelCoordinate:Do()
    if self.value.filepath == nil then
        print('[cluster] filepath of nil');
        return '[cluster] filepath of nil'
    else
        print('[cluster] filepath: ' .. self.value.filepath);
    end
    h, m = io.open(self.value.filepath)
    if h then
        json = h:read('*a')
        if json == nil then
            self.json = ''
            print('[cluster] json of nil');
        else
            self.json = json
            print('[cluster] json input')
        end
    end
    return true
end

