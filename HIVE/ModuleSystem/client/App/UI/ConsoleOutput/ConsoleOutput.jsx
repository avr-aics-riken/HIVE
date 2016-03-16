import React from 'react'
import ReactDOM from 'react-dom'
import Constants from '../../Core/Constants.jsx'

export default class ConsoleOutput extends React.Component {
    constructor(props) {
        super(props);
        this.props = props;
        
        this.state = {
            consoletext: 'This console output'
        }
        this.props.store.on(Constants.RENDERER_LOG_RECIEVED, (data) => {
            const newtext = data; 
            const oldtext = this.state.consoletext;
            this.setState({consoletext: oldtext + newtext});
        });
    }
    
    componentDidUpdate() {
        const node = ReactDOM.findDOMNode(this);
        node.scrollTop = node.scrollHeight
    }
    
    styles(show) {
        return {
            position: "absolute",
            color: "#CCC",
            backgroundColor: "rgba(50,50,50, 0.8)",
            width: "600px",  
            height: "500px",
            zIndex: "100",
            overflow: "auto",
            display: (show ? "block" : "none")
        };
    }
    render () {        
        const style = this.styles(this.props.show);
        return (<div style={style}><pre>{this.state.consoletext}</pre></div>);
    }
}
