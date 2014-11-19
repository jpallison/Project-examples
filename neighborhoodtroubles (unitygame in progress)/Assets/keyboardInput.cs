using UnityEngine;
using System.Collections;

public class keyboardInput : MonoBehaviour {
	public float moveSpeed;
	bool facingRight = true;
	void Start ()
	{
	
	}

	void Update () {
		movement ();
	}
	void movement()
	{

		if (Input.GetKey (KeyCode.A)) 
		{
			transform.Translate(new Vector3(-moveSpeed, 0, 0));
			if (facingRight)
			{
				Flip ();
			}
			facingRight = false;
		}
		if (Input.GetKey (KeyCode.D)) 
		{
			transform.Translate(new Vector3(moveSpeed, 0, 0));
			if(!facingRight)
			{
				Flip ();
			}
			facingRight = true;
		}
		//if (Input.GetKey (KeyCode.W)) 
		//{
		//	transform.Translate(new Vector3(0,moveSpeed, 0));
		//}

		if (Input.GetKey (KeyCode.S)) 
		{
			transform.Translate(new Vector3(0,-moveSpeed, 0));
		}
		//if (temp = temp && !facingRight) 
		//		Flip ();
		//else if (temp = temp && facingRight)
		//		Flip ();
	
	}


	void Flip()
	{
		facingRight = !facingRight;
		Vector3 theScale = transform.localScale;
		theScale.x *= -1;
		transform.localScale = theScale;

	}
}