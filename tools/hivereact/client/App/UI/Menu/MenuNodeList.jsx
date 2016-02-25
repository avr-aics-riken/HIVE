import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

export default class MenuNodeList extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes : null,
        };

        this.styles = this.styles.bind(this);
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
            }
        }
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.block}>
            </div>
        );
    }
}
