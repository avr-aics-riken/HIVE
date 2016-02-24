
import React from 'react'

class RenderView extends React.Component {
	constructor(props) {
        super(props);
        console.log('RenderView Constructor:', props);
		
        this.node = props.node;
        this.action = props.action;
        
        this.state = {
            text: ''
        }
	}
    
    

    onChange0(event) {
        let text = event.target.value;
        this.setState({text});
        this.node.input[5].value[0] = parseFloat(text);        
        this.action.changeNode(this.node);
    }
    onChange1(event) {
        let text = event.target.value;
        this.setState({text});
        this.node.input[5].value[1] = parseFloat(text);        
        this.action.changeNode(this.node);
    }
    onChange2(event) {
        let text = event.target.value;
        this.setState({text});
        this.node.input[5].value[2] = parseFloat(text);        
        this.action.changeNode(this.node);
    }
    
    render(){        
        return (
            <div>
                <div>
                <p>R:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange0.bind(this)}
                    defaultValue={this.node.input[5].value[0]}/>
                </div>
                <div>
                <p>G:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange1.bind(this)}
                    defaultValue={this.node.input[5].value[1]}/>
                </div>
                <div>
                <p>B:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange2.bind(this)}
                    defaultValue={this.node.input[5].value[2]}/>
                </div>
            </div>
        );
    }
}

module.exports = RenderView;
