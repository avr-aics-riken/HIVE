
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
            text: ''
        }
    }

    onChange(event) {
        let text = event.target.value;
        this.setState({text});
        this.node.input[0].value[2] = parseFloat(text);
        //console.log('AAA', this.node);
        this.action.changeNode(this.node);
    }
    
    render(){
        // return (<div>test</div>);
        return (
            <div>
             <p>Z:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                onChange = {this.onChange.bind(this)}
                defaultValue={this.node.input[0].value[2]}/>
            </div>
        );
    }
}

module.exports = CreateCamera;
