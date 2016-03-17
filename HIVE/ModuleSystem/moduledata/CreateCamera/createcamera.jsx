
console.log('!!createcamera.jsx!!');

import React from 'react'
//var React = require('react');

class CreateCamera extends React.Component {
    constructor(props) {
        super(props);
        console.log("CAMERA CONSTRUCTOR!", props);
        this.node = props.node;
        this.action = props.action;

        this.state = {
            text: [
                this.node.input[0].value[0],
                this.node.input[0].value[1],
                this.node.input[0].value[2]
            ]
        }
    }

    onChange(i, event) {
        let text = event.target.value;
        let st = this.state.text.slice(0);
        st[i] = text;
        this.setState({text:st});

        const inputs = JSON.parse(JSON.stringify(this.node.input));
        inputs[0].value[i] = parseFloat(text);
        const varname = this.node.varname;
        this.action.changeNode({
            varname: varname,
            input: inputs
        });
    }

    render(){
        return (
            <div>
            <p>POS X:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                onChange = {this.onChange.bind(this,0)}
                value={this.node.input[0].value[0]}
                defaultValue={this.node.input[0].value[0]}/>
            <p>POS Y:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                onChange = {this.onChange.bind(this,1)}
                value={this.node.input[0].value[1]}
                defaultValue={this.node.input[0].value[1]}/>
             <p>POS Z:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                onChange = {this.onChange.bind(this,2)}
                value={this.node.input[0].value[2]}
                defaultValue={this.node.input[0].value[2]}/>
            </div>
        );
    }
}

module.exports = CreateCamera;
