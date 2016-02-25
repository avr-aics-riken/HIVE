import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

export default class MenuNodeList extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes: null,
            nodeList: this.props.store.getNodeNameList()
        };

        this.styles = this.styles.bind(this);
        this.props.store.on(Core.Constants.INITIALIZED, ()=>{
            this.setState({nodeList: this.props.store.getNodeNameList()});
        }.bind(this));
    }

    styles() {
        return {
            block: {
                backgroundColor: "rgba(0, 0, 0, 0.25)",
                margin: "0px",
                padding: "0px",
                width: "100%",
                height: "300px",
                boxShadow: "0px 0px 1px 0px white inset"
            },
            list: {
                lineHeight: "30px",
                width: "100%",
                height: "30px",
                boxShadow: "0px 0px 1px 0px white inset"
            }
        }
    }

    generator(value, key){
        const style = this.styles();
        return (
            <div style={style.list} key={key}>{value}</div>
        );
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.block}>
                {this.state.nodeList.map(this.generator.bind(this))}
            </div>
        );
    }
}
